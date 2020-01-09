RWStructuredBuffer<float> CurrentLum : register( u0 );
RWStructuredBuffer<float> PrevLum : register( u1 );

cbuffer CBSumm : register( b0 )
{
	uint1	SummSize;
	float1	ElapsedTime;
	float1	NumPoint;
}

[numthreads( 1, 1, 1 )]
void CS_CalcAdaptedLum( uint GroupIndex : SV_GroupIndex )
{
	if ( GroupIndex > 0 )
		InterlockedAdd( CurrentLum[0], CurrentLum[GroupIndex], CurrentLum[0] );

	AllMemoryBarrier();

	CurrentLum[0] = CurrentLum[0] / NumPoint;
	CurrentLum[0] = PrevLum[0] + ( CurrentLum[0] - PrevLum[0] ) * ( 1 - pow( 0.98f, 45.0f * ElapsedTime ) );
	PrevLum[0] = CurrentLum[0];
}