
// Range: [0, 0.5]
#define SMAA_PREDICATION_THRESHOLD 0.01f

// Range: [1, 5]
#define SMAA_PREDICATION_SCALE 2.0f

// Range: [0, 1]
#define SMAA_PREDICATION_STRENGTH 0.4f

#define SMAA_DEPTH_THRESHOLD ( 0.01 * SMAA_THRESHOLD )





SamplerState LinearSampler { Filter = MIN_MAG_LINEAR_MIP_POINT; AddressU = Clamp; AddressV = Clamp; };
SamplerState PointSampler { Filter = MIN_MAG_MIP_POINT; AddressU = Clamp; AddressV = Clamp; };
#define SMAATexture2D Texture2D
#define SMAASampleLevelZero(tex, coord) tex.SampleLevel(LinearSampler, coord, 0)
#define SMAASampleLevelZeroPoint(tex, coord) tex.SampleLevel(PointSampler, coord, 0)
#define SMAASample(tex, coord) SMAASampleLevelZero(tex, coord)
#define SMAASamplePoint(tex, coord) SMAASampleLevelZeroPoint(tex, coord)
#define SMAASampleLevelZeroOffset(tex, coord, off) tex.SampleLevel(LinearSampler, coord, 0, off)
#define SMAASampleOffset(tex, coord, off) SMAASampleLevelZeroOffset(tex, coord, off)
#define SMAAGather(tex, coord) tex.Gather(LinearSampler, coord, 0)


#define SMAA_PIXEL_SIZE float2(1.0 / 1280.0, 1.0 / 720.0)


#define SMAA_AREATEX_MAX_DISTANCE 16
#define SMAA_AREATEX_MAX_DISTANCE_DIAG 20
#define SMAA_AREATEX_PIXEL_SIZE ( 1.0f / float2( 160.0f, 240.0f ) )
#define SMAA_AREATEX_SUBTEX_SIZE ( 1.0f / 3.0f )

#define SMAA_PRESET_ULTRA 1
#if SMAA_PRESET_LOW == 1
#define SMAA_THRESHOLD 0.15
#define SMAA_MAX_SEARCH_STEPS 4
#define SMAA_MAX_SEARCH_STEPS_DIAG 0
#define SMAA_CORNER_ROUNDING 100
#elif SMAA_PRESET_MEDIUM == 1
#define SMAA_THRESHOLD 0.1
#define SMAA_MAX_SEARCH_STEPS 8
#define SMAA_MAX_SEARCH_STEPS_DIAG 0
#define SMAA_CORNER_ROUNDING 100
#elif SMAA_PRESET_HIGH == 1
#define SMAA_THRESHOLD 0.1
#define SMAA_MAX_SEARCH_STEPS 16
#define SMAA_MAX_SEARCH_STEPS_DIAG 8
#define SMAA_CORNER_ROUNDING 25
#elif SMAA_PRESET_ULTRA == 1
//#define SMAA_THRESHOLD 0.05
#define SMAA_THRESHOLD 0.2
//#define SMAA_MAX_SEARCH_STEPS 32
#define SMAA_MAX_SEARCH_STEPS 98
//#define SMAA_MAX_SEARCH_STEPS_DIAG 16
#define SMAA_MAX_SEARCH_STEPS_DIAG 17
//#define SMAA_CORNER_ROUNDING 25
#define SMAA_CORNER_ROUNDING 100
#endif

inline float2 SMAACalculatePredicatedThreshold( float2 texcoord, float4 offset[3], Texture2D colorTex, Texture2D DepthTex )
{
    float3 neighbours = SMAAGather( DepthTex, texcoord + PixelSize * float2(-0.5, -0.5) ).grb;
    float2 delta = abs( neighbours.xx - float2(neighbours.y, neighbours.z) );
    float2 edges = step( SMAA_PREDICATION_THRESHOLD, delta );
    return SMAA_PREDICATION_SCALE * SMAA_THRESHOLD * ( 1.0f - SMAA_PREDICATION_STRENGTH * edges );
}

float4 SMAALumaEdgeDetectionPS(	float4 position		: SV_POSITION,
								float2 texcoord		: TEXCOORD0,
								float4 offset[3]	: TEXCOORD1 ) : SV_Target
{
	float2 threshold = SMAACalculatePredicatedThreshold( texcoord, offset, g_DiffuseTexture, g_DepthTexture );

    float3 weights = float3(0.2126, 0.7152, 0.0722);
    float L = dot(SMAASample( g_DiffuseTexture, texcoord).rgb, weights);
    float Lleft = dot(SMAASample( g_DiffuseTexture, offset[0].xy).rgb, weights);
    float Ltop  = dot(SMAASample( g_DiffuseTexture, offset[0].zw).rgb, weights);

    float4 delta;
    delta.xy = abs(L.xx - float2(Lleft, Ltop));
    float2 edges = step(threshold, delta.xy);
    if (dot(edges, 1.0) == 0.0)
        return float4( 0.0f, 0.0f, 0.0f, 0.0f );

    float Lright = dot(SMAASample( g_DiffuseTexture, offset[1].xy).rgb, weights);
    float Lbottom  = dot(SMAASample( g_DiffuseTexture, offset[1].zw).rgb, weights);
    delta.zw = abs(L.xx - float2(Lright, Lbottom));

    float2 maxDelta = max(max(max(delta.x, delta.y), delta.z), delta.w);

    float Lleftleft = dot(SMAASample( g_DiffuseTexture, offset[2].xy).rgb, weights);
    float Ltoptop = dot(SMAASample( g_DiffuseTexture, offset[2].zw).rgb, weights);
    delta.zw = abs(float2(Lleft, Ltop) - float2(Lleftleft, Ltoptop));

    maxDelta = float2(max(maxDelta.x, delta.z), max(maxDelta.y, delta.w));

    edges.xy *= step( 0.5 * maxDelta, delta.xy );

    return float4( edges, 0.0, 0.0 );
}

void SMAAEdgeDetectionVS(			float4 position		: POSITION,
							out		float4 svPosition	: SV_POSITION,
							inout	float2 texcoord		: TEXCOORD0,
							out		float4 offset[3]	: TEXCOORD1 )
{
    //svPosition = position;

    //offset[0] = texcoord.xyxy + PixelSize.xyxy * float4( -1.0f, 0.0f, 0.0f, -1.0f );
    //offset[1] = texcoord.xyxy + PixelSize.xyxy * float4(  1.0f, 0.0f, 0.0f,  1.0f );
    //offset[2] = texcoord.xyxy + PixelSize.xyxy * float4( -2.0f, 0.0f, 0.0f, -2.0f );


    svPosition = position;

    offset[0] = texcoord.xyxy + SMAA_PIXEL_SIZE.xyxy * float4(-1.0, 0.0, 0.0, -1.0);
    offset[1] = texcoord.xyxy + SMAA_PIXEL_SIZE.xyxy * float4( 1.0, 0.0, 0.0,  1.0);
    offset[2] = texcoord.xyxy + SMAA_PIXEL_SIZE.xyxy * float4(-2.0, 0.0, 0.0, -2.0);
}

technique11 SMAALumaEdgeDetection 
{
    pass LumaEdgeDetection
	{
        SetVertexShader( CompileShader( vs_5_0, SMAAEdgeDetectionVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, SMAALumaEdgeDetectionPS() ) );

		SetDepthStencilState( DisableDepthReplaceStencil, 1 );
		SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

float4 SMAADepthEdgeDetectionPS(	float4 position		: SV_POSITION,
									float2 texcoord		: TEXCOORD0,
									float4 offset[3]	: TEXCOORD1 ) : SV_Target
{
	float3 neighbours = SMAAGather( g_DepthTexture, texcoord + PixelSize * float2(-0.5, -0.5) ).grb;
	float2 delta = abs( neighbours.xx - float2( neighbours.y, neighbours.z ) );
	float2 edges = step( SMAA_DEPTH_THRESHOLD, delta );

    if ( dot(edges, 1.0 ) == 0.0 )
        discard;

    return float4( edges, 0.0, 0.0 );
}

technique11 SMAADepthEdgeDetection
{
    pass DepthEdgeDetection
	{
        SetVertexShader( CompileShader( vs_5_0, SMAAEdgeDetectionVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, SMAADepthEdgeDetectionPS() ) );

        SetDepthStencilState( DisableDepthReplaceStencil, 1 );
        SetBlendState( NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF );
    }
}

//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

float4 SMAAColorEdgeDetectionPS(	float4 position		: SV_POSITION,
                                    float2 texcoord		: TEXCOORD0,
                                    float4 offset[3]	: TEXCOORD1 ) : SV_Target
{
    float2 threshold = SMAACalculatePredicatedThreshold( texcoord, offset, g_DiffuseTexture, g_DepthTexture );

    // Calculate color deltas:
    float4 delta;
    float3 C = SMAASample(g_DiffuseTexture, texcoord).rgb;

    float3 Cleft = SMAASample(g_DiffuseTexture, offset[0].xy).rgb;
    float3 t = abs(C - Cleft);
    delta.x = max(max(t.r, t.g), t.b);

    float3 Ctop  = SMAASample(g_DiffuseTexture, offset[0].zw).rgb;
    t = abs(C - Ctop);
    delta.y = max(max(t.r, t.g), t.b);

    // We do the usual threshold:
    float2 edges = step(threshold, delta.xy);

    // Then discard if there is no edge:
    if (dot(edges, 1.0) == 0.0)
        discard;

    // Calculate right and bottom deltas:
    float3 Cright = SMAASample(g_DiffuseTexture, offset[1].xy).rgb;
    t = abs(C - Cright);
    delta.z = max(max(t.r, t.g), t.b);

    float3 Cbottom  = SMAASample(g_DiffuseTexture, offset[1].zw).rgb;
    t = abs(C - Cbottom);
    delta.w = max(max(t.r, t.g), t.b);

    // Calculate the maximum delta in the direct neighborhood:
    float maxDelta = max(max(max(delta.x, delta.y), delta.z), delta.w);

    // Calculate left-left and top-top deltas:
    float3 Cleftleft  = SMAASample(g_DiffuseTexture, offset[2].xy).rgb;
    t = abs(C - Cleftleft);
    delta.z = max(max(t.r, t.g), t.b);

    float3 Ctoptop = SMAASample(g_DiffuseTexture, offset[2].zw).rgb;
    t = abs(C - Ctoptop);
    delta.w = max(max(t.r, t.g), t.b);

    // Calculate the final maximum delta:
    maxDelta = max(max(maxDelta, delta.z), delta.w);

    // Local contrast adaptation in action:
    edges.xy *= step(0.5 * maxDelta, delta.xy);

    return float4(edges, 0.0, 0.0);
}

technique11 SMAAColorEdgeDetection
{
    pass ColorEdgeDetection
	{
        SetVertexShader( CompileShader( vs_5_0, SMAAEdgeDetectionVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, SMAAColorEdgeDetectionPS() ) );

        SetDepthStencilState( DisableDepthReplaceStencil, 1 );
        SetBlendState( NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF );
    }
}

//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

float2 SMAAAreaDiag( Texture2D areaTex, float2 distance, float2 e, float offset )
{
    float2 texcoord = SMAA_AREATEX_MAX_DISTANCE_DIAG * e + distance;

    texcoord = SMAA_AREATEX_PIXEL_SIZE * texcoord + (0.5 * SMAA_AREATEX_PIXEL_SIZE);
    texcoord.x += 0.5;
    texcoord.y += SMAA_AREATEX_SUBTEX_SIZE * offset;

    return SMAASampleLevelZero( areaTex, texcoord ).rg;
}

float SMAASearchDiag1( Texture2D edgesTex, float2 texcoord, float2 dir, float c )
{
    texcoord += dir * PixelSize;
    float2 e = 0;

	[unrol]
    for ( float i = 0; i < SMAA_MAX_SEARCH_STEPS_DIAG; i++ )
	{
        e.rg = SMAASampleLevelZero(edgesTex, texcoord).rg;
        [flatten] if (dot(e, 1.0) < 1.9) break;
        texcoord += dir * PixelSize;
    }

    return i + float(e.g > 0.9) * c;
}

float SMAASearchDiag2( Texture2D edgesTex, float2 texcoord, float2 dir, float c )
{
    texcoord += dir * PixelSize;
    float2 e = 0;
    for (float i = 0; i < SMAA_MAX_SEARCH_STEPS_DIAG; i++) {
        e.g = SMAASampleLevelZero(edgesTex, texcoord).g;
        e.r = SMAASampleLevelZeroOffset(edgesTex, texcoord, int2(1, 0)).r;
        [flatten] if (dot(e, 1.0) < 1.9) break;
        texcoord += dir * PixelSize;
    }
    return i + float(e.g > 0.9) * c;
}

float2 SMAACalculateDiagWeights( Texture2D edgesTex, Texture2D areaTex, float2 texcoord, float2 e, int4 SubsampleIndices )
{
    float2 weights = 0.0;

    float2 d;
    d.x = e.r? SMAASearchDiag1(edgesTex, texcoord, float2(-1.0,  1.0), 1.0) : 0.0;
    d.y = SMAASearchDiag1(edgesTex, texcoord, float2(1.0, -1.0), 0.0);

    [branch]
    if (d.r + d.g > 2) { // d.r + d.g + 1 > 3
        float4 coords = mad(float4(-d.r, d.r, d.g, -d.g), PixelSize.xyxy, texcoord.xyxy);

        float4 c;
        c.x = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2(-1,  0)).g;
        c.y = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2( 0,  0)).r;
        c.z = SMAASampleLevelZeroOffset(edgesTex, coords.zw, int2( 1,  0)).g;
        c.w = SMAASampleLevelZeroOffset(edgesTex, coords.zw, int2( 1, -1)).r;
        float2 e = 2.0 * c.xz + c.yw;
        e *= step(d.rg, SMAA_MAX_SEARCH_STEPS_DIAG - 1);

        weights += SMAAAreaDiag(areaTex, d, e, SubsampleIndices.z);
    }

    d.x = SMAASearchDiag2(edgesTex, texcoord, float2(-1.0, -1.0), 0.0);
    float right = SMAASampleLevelZeroOffset(edgesTex, texcoord, int2(1, 0)).r;
    d.y = right? SMAASearchDiag2(edgesTex, texcoord, float2(1.0, 1.0), 1.0) : 0.0;

    [branch]
    if (d.r + d.g > 2) { // d.r + d.g + 1 > 3
        float4 coords = mad(float4(-d.r, -d.r, d.g, d.g), PixelSize.xyxy, texcoord.xyxy);

        float4 c;
        c.x  = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2(-1,  0)).g;
        c.y  = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2( 0, -1)).r;
        c.zw = SMAASampleLevelZeroOffset(edgesTex, coords.zw, int2( 1,  0)).gr;
        float2 e = 2.0 * c.xz + c.yw;
        e *= step(d.rg, SMAA_MAX_SEARCH_STEPS_DIAG - 1);

        weights += SMAAAreaDiag(areaTex, d, e, SubsampleIndices.w).gr;
    }

    return weights;
}

float SMAASearchLength( Texture2D searchTex, float2 e, float bias, float scale)
{
    e.r = bias + e.r * scale;
    return 255.0 * SMAASampleLevelZeroPoint(searchTex, e).r;
}

float SMAASearchXLeft( Texture2D edgesTex,  Texture2D searchTex, float2 texcoord, float end) {
    float2 e = float2(0.0, 1.0);
    while (texcoord.x > end && 
           e.g > 0.8281 && e.r == 0.0)
	{
        e = SMAASampleLevelZero(edgesTex, texcoord).rg;
        texcoord -= float2(2.0, 0.0) * PixelSize;
    }

    texcoord.x += 0.25 * PixelSize.x;
    texcoord.x += PixelSize.x;
    texcoord.x += 2.0 * PixelSize.x; // Undo last step
    texcoord.x -= PixelSize.x * SMAASearchLength(searchTex, e, 0.0, 0.5);

    return texcoord.x;
}

float SMAASearchXRight( Texture2D edgesTex, Texture2D searchTex, float2 texcoord, float end )
{
    float2 e = float2(0.0, 1.0);
    while (texcoord.x < end && e.g > 0.8281 && e.r == 0.0)
	{
        e = SMAASampleLevelZero(edgesTex, texcoord).rg;
        texcoord += float2(2.0, 0.0) * PixelSize;
    }

    texcoord.x -= 0.25 * PixelSize.x;
    texcoord.x -= PixelSize.x;
    texcoord.x -= 2.0 * PixelSize.x;
    texcoord.x += PixelSize.x * SMAASearchLength( searchTex, e, 0.5, 0.5 );
    return texcoord.x;
}

float2 SMAAArea( Texture2D areaTex, float2 distance, float e1, float e2, float offset )
{
    float2 texcoord = SMAA_AREATEX_MAX_DISTANCE * round(4.0 * float2(e1, e2)) + distance;
    texcoord = SMAA_AREATEX_PIXEL_SIZE * texcoord + (0.5 * SMAA_AREATEX_PIXEL_SIZE);
    texcoord.y += SMAA_AREATEX_SUBTEX_SIZE * offset;

    return SMAASampleLevelZero(areaTex, texcoord).rg;
}

	//int4 SubsampleIndices = 0;
	//int4 SubsampleIndices = float4( 1.0f, 1.0f, 1.0f, 0.0f );
	//int4 SubsampleIndices = float4( 2.0f, 2.0f, 2.0f, 0.0f );

float4 SMAABlendingWeightCalculationPS(	float4 position : SV_POSITION,
                                        float2 texcoord : TEXCOORD0,
                                        float2 pixcoord : TEXCOORD1,
                                        float4 offset[3] : TEXCOORD2 ) : SV_Target
{
	int4 SubsampleIndices = 0;
	//int4 SubsampleIndices = float4( 1.0f, 1.0f, 1.0f, 0.0f );
	//int4 SubsampleIndices = float4( 2.0f, 2.0f, 2.0f, 0.0f );

    float4 weights = 0.0;

    float2 e = SMAASample( g_EdgesTexture, texcoord ).rg;

    [branch]
    if (e.g)
	{
        float2 d;
        float2 coords;
        coords.x = SMAASearchXLeft( g_EdgesTexture, g_SearchTexture, offset[0].xy, offset[2].x );
        coords.y = offset[1].y;
        d.x = coords.x;

        float e1 = SMAASampleLevelZero( g_EdgesTexture, coords ).r;
        coords.x = SMAASearchXRight( g_EdgesTexture, g_SearchTexture, offset[0].zw, offset[2].y );
        d.y = coords.x;
        d = d / PixelSize.x - pixcoord.x;
        float2 sqrt_d = sqrt(abs(d));
        float e2 = SMAASampleLevelZeroOffset( g_EdgesTexture, coords, int2(1, 0) ).r;

        weights.rg = SMAAArea( g_AreaTexture, sqrt_d, e1, e2, SubsampleIndices.y );
	}

    return weights;
}

void SMAABlendWeightCalculationVS(			float4 position		: POSITION,
                                    out		float4 svPosition	: SV_POSITION,
                                    inout	float2 texcoord		: TEXCOORD0,
                                    out		float2 pixcoord		: TEXCOORD1,
                                    out		float4 offset[3]	: TEXCOORD2 )
{
    //svPosition = position;
    //pixcoord = texcoord / PixelSize;

    //offset[0] = texcoord.xyxy + PixelSize.xyxy * float4( -0.25f, -0.125f,  1.25f, -0.125f );
    //offset[1] = texcoord.xyxy + PixelSize.xyxy * float4( -0.125f, -0.25f, -0.125f,  1.25f );

    //offset[2] = float4(offset[0].xz, offset[1].yw) + 
    //            float4(-2.0, 2.0, -2.0, 2.0) *
    //            PixelSize.xxyy * SMAA_MAX_SEARCH_STEPS;


    svPosition = position;

    pixcoord = texcoord / SMAA_PIXEL_SIZE;

    // We will use these offsets for the searches later on (see @PSEUDO_GATHER4):
    offset[0] = texcoord.xyxy + SMAA_PIXEL_SIZE.xyxy * float4(-0.25, -0.125,  1.25, -0.125);
    offset[1] = texcoord.xyxy + SMAA_PIXEL_SIZE.xyxy * float4(-0.125, -0.25, -0.125,  1.25);

    // And these for the searches, they indicate the ends of the loops:
    offset[2] = float4(offset[0].xz, offset[1].yw) + 
                float4(-2.0, 2.0, -2.0, 2.0) *
                SMAA_PIXEL_SIZE.xxyy * SMAA_MAX_SEARCH_STEPS;
}

technique11 SMAABlendingWeightCalculation
{
    pass BlendingWeightCalculation
	{
        SetVertexShader( CompileShader( vs_5_0, SMAABlendWeightCalculationVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, SMAABlendingWeightCalculationPS() ) );

		SetDepthStencilState( DisableDepthUseStencil, 1 );
		SetBlendState( NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF );
    }
}

//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

float4 SMAANeighborhoodBlendingPS(	float4 position		: SV_POSITION,
                                    float2 texcoord		: TEXCOORD0,
                                    float4 offset[2]	: TEXCOORD1 ) : SV_Target
                                  //SMAATexture2D colorTex,
                                  //SMAATexture2D blendTex)
{
    float4 topLeft = SMAASample( g_BlendTexture, texcoord );
    float bottom = SMAASample( g_BlendTexture, offset[1].zw ).g;
    float right = SMAASample( g_BlendTexture, offset[1].xy ).a;
    float4 a = float4( topLeft.r, bottom, topLeft.b, right );

    // Is there any blending weight with a value greater than 0.0?
    [branch]
    if (dot(a, 1.0) < 1e-5)
        return SMAASampleLevelZero( g_ColorTexture, texcoord );
    else
	{
        float4 color = 0.0;
        float2 offset;
        offset.x = a.a > a.b? a.a : -a.b; // left vs. right 
        offset.y = a.g > a.r? a.g : -a.r; // top vs. bottom

        if ( abs( offset.x ) > abs( offset.y ) ) // horizontal vs. vertical
            offset.y = 0.0;
        else
            offset.x = 0.0;

        float4 C = SMAASampleLevelZero( g_ColorTexture, texcoord );
        texcoord += sign(offset) * PixelSize;
        float4 Cop = SMAASampleLevelZero( g_ColorTexture, texcoord );
        float s = abs(offset.x) > abs(offset.y) ? abs(offset.x) : abs(offset.y);
        return lerp( C, Cop, s );
    }
}

void SMAANeighborhoodBlendingVS(			float4 position		: POSITION,
                                    out		float4 svPosition	: SV_POSITION,
                                    inout	float2 texcoord		: TEXCOORD0,
                                    out		float4 offset[2]	: TEXCOORD1 )
{
    //svPosition = position;

    //offset[0] = texcoord.xyxy + PixelSize.xyxy * float4( -1.0f, 0.0f, 0.0f, -1.0f );
    //offset[1] = texcoord.xyxy + PixelSize.xyxy * float4(  1.0f, 0.0f, 0.0f,  1.0f );

    svPosition = position;

    offset[0] = texcoord.xyxy + SMAA_PIXEL_SIZE.xyxy * float4(-1.0, 0.0, 0.0, -1.0);
    offset[1] = texcoord.xyxy + SMAA_PIXEL_SIZE.xyxy * float4( 1.0, 0.0, 0.0,  1.0);
}

// Neighborhood blending technique
technique11 SMAANeighborhoodBlending
{
    pass NeighborhoodBlending
	{
        SetVertexShader( CompileShader( vs_5_0, SMAANeighborhoodBlendingVS() ) );
        SetGeometryShader(NULL);
        SetPixelShader( CompileShader( ps_5_0, SMAANeighborhoodBlendingPS() ) );

        SetDepthStencilState( DisableDepthStencil, 0 );
        SetBlendState( NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF );
    }
}

