//////////////////////////////////////////////////////
// Подсчёт общей суммы

StructuredBuffer<float> SummInput : register( t0 );
RWStructuredBuffer<float> SummResult : register( u0 );

cbuffer CBSumm : register( b0 )
{
	uint1	SummSize;
	float1	ElapsedTime;
	float1	NumPoint;
}

#define SummHDR_BlockSize 1024

groupshared float summ[SummHDR_BlockSize];

[numthreads( SummHDR_BlockSize, 1, 1 )]
void CS_CalcSummLum( uint3 DispatchThreadID : SV_DispatchThreadID, uint3 GroupThreadID : SV_GroupThreadID, uint GroupIndex : SV_GroupIndex )
{
	if ( DispatchThreadID.x < SummSize )
		summ[GroupThreadID.x] = SummInput[DispatchThreadID.x];
	else
		summ[GroupThreadID.x] = 0;

	GroupMemoryBarrierWithGroupSync();
    if ( GroupThreadID.x < 512 )
		summ[GroupThreadID.x] += summ[GroupThreadID.x + 512];

	GroupMemoryBarrierWithGroupSync();
    if ( GroupThreadID.x < 256 )
		summ[GroupThreadID.x] += summ[GroupThreadID.x + 256];

	GroupMemoryBarrierWithGroupSync();
    if ( GroupThreadID.x < 128 )
		summ[GroupThreadID.x] += summ[GroupThreadID.x + 128];

	GroupMemoryBarrierWithGroupSync();
    if ( GroupThreadID.x < 64 )
		summ[GroupThreadID.x] += summ[GroupThreadID.x + 64];

	GroupMemoryBarrierWithGroupSync();
    if ( GroupThreadID.x < 32 )
		summ[GroupThreadID.x] += summ[GroupThreadID.x + 32];

	GroupMemoryBarrierWithGroupSync();
    if ( GroupThreadID.x < 16 )
		summ[GroupThreadID.x] += summ[GroupThreadID.x + 16];

	GroupMemoryBarrierWithGroupSync();
    if ( GroupThreadID.x < 8 )
		summ[GroupThreadID.x] += summ[GroupThreadID.x + 8];

	GroupMemoryBarrierWithGroupSync();
    if ( GroupThreadID.x < 4 )
		summ[GroupThreadID.x] += summ[GroupThreadID.x + 4];

	GroupMemoryBarrierWithGroupSync();
    if ( GroupThreadID.x < 2 )
		summ[GroupThreadID.x] += summ[GroupThreadID.x + 2];

	GroupMemoryBarrierWithGroupSync();
    if ( GroupThreadID.x < 1 )
	{
		summ[GroupThreadID.x] += summ[GroupThreadID.x + 1];
		SummResult[GroupIndex] = summ[0];
	}
}
