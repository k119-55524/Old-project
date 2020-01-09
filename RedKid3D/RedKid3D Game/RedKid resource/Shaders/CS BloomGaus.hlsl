
Texture2D	InBloom				: register( t0 );

RWTexture2D<float4> OutBloom	: register( u0 );

[numthreads( 1, 1, 1 )]
void CS_BloomGaus( uint2 DispatchThreadID : SV_DispatchThreadID )//, uint3 GroupThreadID : SV_GroupThreadID, uint3 GroupID : SV_GroupID )//uint3 DTid : SV_DispatchThreadID )
{
	float4 Color = InBloom[DispatchThreadID.xy] * 0.134598f;

	Color += ( InBloom[DispatchThreadID.xy + float2( 1.0f, 0.0f )] + InBloom[DispatchThreadID.xy - float2( 1.0f, 0.0f )] ) * 0.127325f;
	Color += ( InBloom[DispatchThreadID.xy + float2( 2.0f, 0.0f )] + InBloom[DispatchThreadID.xy - float2( 2.0f, 0.0f )] ) * 0.107778f;
	Color += ( InBloom[DispatchThreadID.xy + float2( 3.0f, 0.0f )] + InBloom[DispatchThreadID.xy - float2( 3.0f, 0.0f )] ) * 0.081638f;
	Color += ( InBloom[DispatchThreadID.xy + float2( 4.0f, 0.0f )] + InBloom[DispatchThreadID.xy - float2( 4.0f, 0.0f )] ) * 0.055335f;
	Color += ( InBloom[DispatchThreadID.xy + float2( 5.0f, 0.0f )] + InBloom[DispatchThreadID.xy - float2( 5.0f, 0.0f )] ) * 0.033562f;
	Color += ( InBloom[DispatchThreadID.xy + float2( 6.0f, 0.0f )] + InBloom[DispatchThreadID.xy - float2( 6.0f, 0.0f )] ) * 0.018216f;
	Color += ( InBloom[DispatchThreadID.xy + float2( 7.0f, 0.0f )] + InBloom[DispatchThreadID.xy - float2( 7.0f, 0.0f )] ) * 0.008847f;

	Color += ( InBloom[DispatchThreadID.xy + float2( 0.0f, 1.0f )] + InBloom[DispatchThreadID.xy - float2( 0.0f, 1.0f )] ) * 0.127325f;
	Color += ( InBloom[DispatchThreadID.xy + float2( 0.0f, 2.0f )] + InBloom[DispatchThreadID.xy - float2( 0.0f, 2.0f )] ) * 0.107778f;
	Color += ( InBloom[DispatchThreadID.xy + float2( 0.0f, 3.0f )] + InBloom[DispatchThreadID.xy - float2( 0.0f, 3.0f )] ) * 0.081638f;
	Color += ( InBloom[DispatchThreadID.xy + float2( 0.0f, 4.0f )] + InBloom[DispatchThreadID.xy - float2( 0.0f, 4.0f )] ) * 0.055335f;
	Color += ( InBloom[DispatchThreadID.xy + float2( 0.0f, 5.0f )] + InBloom[DispatchThreadID.xy - float2( 0.0f, 5.0f )] ) * 0.033562f;
	Color += ( InBloom[DispatchThreadID.xy + float2( 0.0f, 6.0f )] + InBloom[DispatchThreadID.xy - float2( 0.0f, 6.0f )] ) * 0.018216f;
	Color += ( InBloom[DispatchThreadID.xy + float2( 0.0f, 7.0f )] + InBloom[DispatchThreadID.xy - float2( 0.0f, 7.0f )] ) * 0.008847f;


	OutBloom[DispatchThreadID.xy] = float4( Color.xyz, 1.0f );
}