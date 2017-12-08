// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MeshDescription.h"
#include "Misc/SecureHash.h"
#include "Serialization/MemoryWriter.h"
#include "MeshAttributes.h"
#include "Serialization/ObjectWriter.h"

UMeshDescription::UMeshDescription()
{
	// Add basic vertex attributes
	VertexAttributes().RegisterAttribute<FVector>( MeshAttribute::Vertex::Position, 1, FVector::ZeroVector, EMeshAttributeFlags::Lerpable );
	VertexAttributes().RegisterAttribute<float>( MeshAttribute::Vertex::CornerSharpness, 1, 0.0f, EMeshAttributeFlags::Lerpable );

	// Add basic vertex instance attributes
	VertexInstanceAttributes().RegisterAttribute<FVector2D>( MeshAttribute::VertexInstance::TextureCoordinate, 2, FVector2D::ZeroVector, EMeshAttributeFlags::Lerpable );
	VertexInstanceAttributes().RegisterAttribute<FVector>( MeshAttribute::VertexInstance::Normal, 1, FVector::ZeroVector, EMeshAttributeFlags::AutoGenerated );
	VertexInstanceAttributes().RegisterAttribute<FVector>( MeshAttribute::VertexInstance::Tangent, 1, FVector::ZeroVector, EMeshAttributeFlags::AutoGenerated );
	VertexInstanceAttributes().RegisterAttribute<float>( MeshAttribute::VertexInstance::BinormalSign, 1, 0.0f, EMeshAttributeFlags::AutoGenerated );
	VertexInstanceAttributes().RegisterAttribute<FVector4>( MeshAttribute::VertexInstance::Color, 1, FVector4( 1.0f ), EMeshAttributeFlags::Lerpable );

	// Add basic edge attributes
	EdgeAttributes().RegisterAttribute<bool>( MeshAttribute::Edge::IsHard, 1, false );
	EdgeAttributes().RegisterAttribute<float>( MeshAttribute::Edge::CreaseSharpness, 1, 0.0f, EMeshAttributeFlags::Lerpable );

	// Add basic polygon attributes
	PolygonAttributes().RegisterAttribute<FVector>( MeshAttribute::Polygon::Normal, 1, FVector::ZeroVector, EMeshAttributeFlags::AutoGenerated );
	PolygonAttributes().RegisterAttribute<FVector>( MeshAttribute::Polygon::Tangent, 1, FVector::ZeroVector, EMeshAttributeFlags::AutoGenerated );
	PolygonAttributes().RegisterAttribute<FVector>( MeshAttribute::Polygon::Binormal, 1, FVector::ZeroVector, EMeshAttributeFlags::AutoGenerated );
	PolygonAttributes().RegisterAttribute<FVector>( MeshAttribute::Polygon::Center, 1, FVector::ZeroVector, EMeshAttributeFlags::AutoGenerated );

	// Add basic polygon group attributes
	PolygonGroupAttributes().RegisterAttribute<FSoftObjectPath>( MeshAttribute::PolygonGroup::MaterialAsset );
	PolygonGroupAttributes().RegisterAttribute<FName>( MeshAttribute::PolygonGroup::MaterialSlotName );
	PolygonGroupAttributes().RegisterAttribute<FName>( MeshAttribute::PolygonGroup::ImportedMaterialSlotName );
	PolygonGroupAttributes().RegisterAttribute<bool>( MeshAttribute::PolygonGroup::EnableCollision );
	PolygonGroupAttributes().RegisterAttribute<bool>( MeshAttribute::PolygonGroup::CastShadow );
}


void UMeshDescription::Serialize( FArchive& Ar )
{
	Super::Serialize( Ar );

	Ar << VertexArray;
	Ar << VertexInstanceArray;
	Ar << EdgeArray;
	Ar << PolygonArray;
	Ar << PolygonGroupArray;

	Ar << VertexAttributesSet;
	Ar << VertexInstanceAttributesSet;
	Ar << EdgeAttributesSet;
	Ar << PolygonAttributesSet;
	Ar << PolygonGroupAttributesSet;
}


#if WITH_EDITORONLY_DATA
FString UMeshDescription::GetIdString()
{
	TArray<uint8> TempBytes;
	FObjectWriter Ar(this, TempBytes);
	Serialize(Ar);
	FSHA1 Sha;
	TArray<TCHAR> OwnerName = GetPathName().GetCharArray();
	Sha.Update((uint8*)OwnerName.GetData(), OwnerName.Num() * OwnerName.GetTypeSize());
	if (TempBytes.Num() > 0)
	{
		uint8* Buffer = TempBytes.GetData();
		Sha.Update(Buffer, TempBytes.Num());
	}
	Sha.Final();
	TempBytes.Empty();
	// Retrieve the hash and use it to construct a pseudo-GUID.
	uint32 Hash[5];
	Sha.GetHash((uint8*)Hash);
	FGuid Guid = FGuid(Hash[0] ^ Hash[4], Hash[1], Hash[2], Hash[3]);
	return Guid.ToString(EGuidFormats::Digits);
}
#endif


void UMeshDescription::Compact( FElementIDRemappings& OutRemappings )
{
	VertexArray.Compact( OutRemappings.NewVertexIndexLookup );
	VertexInstanceArray.Compact( OutRemappings.NewVertexInstanceIndexLookup );
	EdgeArray.Compact( OutRemappings.NewEdgeIndexLookup );
	PolygonArray.Compact( OutRemappings.NewPolygonIndexLookup );
	PolygonGroupArray.Compact( OutRemappings.NewPolygonGroupIndexLookup );

	RemapAttributes( OutRemappings );
	FixUpElementIDs( OutRemappings );
}


void UMeshDescription::Remap( const FElementIDRemappings& Remappings )
{
	VertexArray.Remap( Remappings.NewVertexIndexLookup );
	VertexInstanceArray.Remap( Remappings.NewVertexInstanceIndexLookup );
	EdgeArray.Remap( Remappings.NewEdgeIndexLookup );
	PolygonArray.Remap( Remappings.NewPolygonIndexLookup );
	PolygonGroupArray.Remap( Remappings.NewPolygonGroupIndexLookup );

	RemapAttributes( Remappings );
	FixUpElementIDs( Remappings );
}


void UMeshDescription::FixUpElementIDs( const FElementIDRemappings& Remappings )
{
	for( const FVertexID VertexID : VertexArray.GetElementIDs() )
	{
		FMeshVertex& Vertex = VertexArray[ VertexID ];

		// Fix up vertex instance index references in vertices array
		for( FVertexInstanceID& VertexInstanceID : Vertex.VertexInstanceIDs )
		{
			VertexInstanceID = Remappings.GetRemappedVertexInstanceID( VertexInstanceID );
		}

		// Fix up edge index references in the vertex array
		for( FEdgeID& EdgeID : Vertex.ConnectedEdgeIDs )
		{
			EdgeID = Remappings.GetRemappedEdgeID( EdgeID );
		}
	}

	// Fix up vertex index references in vertex instance array
	for( const FVertexInstanceID VertexInstanceID : VertexInstanceArray.GetElementIDs() )
	{
		FMeshVertexInstance& VertexInstance = VertexInstanceArray[ VertexInstanceID ];

		VertexInstance.VertexID = Remappings.GetRemappedVertexID( VertexInstance.VertexID );

		for( FPolygonID& PolygonID : VertexInstance.ConnectedPolygons )
		{
			PolygonID = Remappings.GetRemappedPolygonID( PolygonID );
		}
	}

	for( const FEdgeID EdgeID : EdgeArray.GetElementIDs() )
	{
		FMeshEdge& Edge = EdgeArray[ EdgeID ];

		// Fix up vertex index references in Edges array
		for( int32 Index = 0; Index < 2; Index++ )
		{
			Edge.VertexIDs[ Index ] = Remappings.GetRemappedVertexID( Edge.VertexIDs[ Index ] );
		}

		// Fix up references to section indices
		for( FPolygonID& ConnectedPolygon : Edge.ConnectedPolygons )
		{
			ConnectedPolygon = Remappings.GetRemappedPolygonID( ConnectedPolygon );
		}
	}

	for( const FPolygonID PolygonID : PolygonArray.GetElementIDs() )
	{
		FMeshPolygon& Polygon = PolygonArray[ PolygonID ];

		// Fix up references to vertex indices in section polygons' contours
		for( FVertexInstanceID& VertexInstanceID : Polygon.PerimeterContour.VertexInstanceIDs )
		{
			VertexInstanceID = Remappings.GetRemappedVertexInstanceID( VertexInstanceID );
		}

		for( FMeshPolygonContour& HoleContour : Polygon.HoleContours )
		{
			for( FVertexInstanceID& VertexInstanceID : HoleContour.VertexInstanceIDs )
			{
				VertexInstanceID = Remappings.GetRemappedVertexInstanceID( VertexInstanceID );
			}
		}

		for( FMeshTriangle& Triangle : Polygon.Triangles )
		{
			for( int32 TriangleVertexNumber = 0; TriangleVertexNumber < 3; ++TriangleVertexNumber )
			{
				const FVertexInstanceID OriginalVertexInstanceID = Triangle.GetVertexInstanceID( TriangleVertexNumber );
				const FVertexInstanceID NewVertexInstanceID = Remappings.GetRemappedVertexInstanceID( OriginalVertexInstanceID );
				Triangle.SetVertexInstanceID( TriangleVertexNumber, NewVertexInstanceID );
			}
		}

		Polygon.PolygonGroupID = Remappings.GetRemappedPolygonGroupID( Polygon.PolygonGroupID );
	}

	for( const FPolygonGroupID PolygonGroupID : PolygonGroupArray.GetElementIDs() )
	{
		FMeshPolygonGroup& PolygonGroup = PolygonGroupArray[ PolygonGroupID ];

		for( FPolygonID& Polygon : PolygonGroup.Polygons )
		{
			Polygon = Remappings.GetRemappedPolygonID( Polygon );
		}
	}
}


void UMeshDescription::RemapAttributes( const FElementIDRemappings& Remappings )
{
	VertexAttributesSet.Remap( Remappings.NewVertexIndexLookup );
	VertexInstanceAttributesSet.Remap( Remappings.NewVertexInstanceIndexLookup );
	EdgeAttributesSet.Remap( Remappings.NewEdgeIndexLookup );
	PolygonAttributesSet.Remap( Remappings.NewPolygonIndexLookup );
	PolygonGroupAttributesSet.Remap( Remappings.NewPolygonGroupIndexLookup );
}
