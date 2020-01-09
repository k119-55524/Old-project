Shader "Saratov/shader_00"
{
	Properties
	{
		_MainTex( "Albedo texture(RGB)", 2D ) = "white" {}
		_MatrixTex( "Matrix texture(RGB)", 2D ) = "white" {}
		_NoiseTex( "Noise texture(RGB)", 2D ) = "white" {}
		_ScrollSpeed( "Scroll texture speed", Range( -25, 25  ) ) = 0.1
		_CenterSize( "Center size %", Range( 50, 95 ) ) = 80
	}

	SubShader
	{
		//Tags{ "RenderType" = "Opaque" }
		Tags{ "Queue" = "Transparent"  "RenderType" = "Opaque" }
		//ZWrite Off
		//Cull Back
		//Cull Front

		LOD 200
		
		CGPROGRAM

		#pragma surface surf Lambert alpha

		// Use shader model 3.0 target, to get nicer looking lighting
		#pragma target 3.0

		sampler2D _MainTex;
		sampler2D _MatrixTex;
		sampler2D _NoiseTex;

		fixed3 _PosMatrixObj;
		fixed _ScrollSpeed;
		fixed _RadMatrixObj;
		fixed _CenterSize;

		struct Input
		{
			float2 uv_MainTex;
			float3 worldPos;
			//float4 screenPos;
			float3 viewDir;
		};

		// Add instancing support for this shader. You need to check 'Enable Instancing' on materials that use the shader.
		// See https://docs.unity3d.com/Manual/GPUInstancing.html for more information about instancing.
		// #pragma instancing_options assumeuniformscaling
		UNITY_INSTANCING_CBUFFER_START(Props)
			// put more per-instance properties here
		UNITY_INSTANCING_CBUFFER_END

		const fixed textureAuraSize = 512;
		const fixed textureAuraUVCoef = 0.001953125; // 1.0 / textureAuraSize;

		// Проекция вектора на вектор
		float3 Project(float3 v1, float3 v2)
		{
			float3 nv1 = normalize(v1);
			return nv1 * dot(nv1, v2);
		}

		fixed4 GetNoiseColor(fixed dist, fixed4 albedoColor, fixed4 matrixColor, float3 viewDir)
		{
			fixed len = _RadMatrixObj - _RadMatrixObj / 100.0 * (fixed)_CenterSize;
			fixed delta = 1.0 / len;

			//fixed4 col1 = fixed4(0.5, 0.5, 0.5, 1.0);
			matrixColor.a = 0.5;
			fixed4 col = lerp(albedoColor, matrixColor, (_RadMatrixObj - dist) * delta);

			//uv -= _Time.y * 0.01;// *_ScrollSpeed;
			//uv *= 0.1;
			//fixed4 auraColor = tex2D(_NoiseTex, uv);
			//auraColor.a = 1.0 - ( auraColor.r + auraColor.g + auraColor.b ) / 3.0;
			//auraColor.rgb *= auraColor.a;
			//auraColor.a = 0.0;
			//col = auraColor;
			//col += auraColor;

			return col;
		}

		void surf(Input IN, inout SurfaceOutput o)
		{
			float dist = distance(IN.worldPos, _PosMatrixObj);
			fixed4 albedo = tex2D(_MainTex, IN.uv_MainTex);
			if (dist >= _RadMatrixObj)
			{
				o.Albedo = albedo.rgb;
				o.Alpha = albedo.a;
			}
			else
			{
				fixed2 UV = IN.uv_MainTex;
				UV.y -= _ScrollSpeed * _Time.y;
				fixed4 colMatr = tex2D(_MatrixTex, UV);

				if ((_RadMatrixObj / 100.0 * _CenterSize) > dist)
				{
					o.Albedo = colMatr.rgb;
					o.Alpha = 0.5;
				}
				else
				{
					//////////////////////////////////////////////////////////
					float3 proj = Project(IN.viewDir, IN.worldPos);
					float2 tuv = IN.worldPos.xy - proj.xy;
					tuv.y -= 1.0 * _Time.y;
					//tuv *= textureAuraUVCoef;
					fixed4 colArea = tex2D(_NoiseTex, tuv);

					float diam = 100.0 - _CenterSize;
					float rad = diam / 2.0f;
					float k = 1.0 / diam;
					float dp = 100.0 / _RadMatrixObj * dist;
					float dr = abs(100 - dp - rad);
					float alf = sqrt(rad * rad - dr * dr);
					colArea.rgb *= alf * k * 0.3;
					//colArea.a = alf * (1.0 / rad);
					//colArea.a = (colArea.r + colArea.g + colArea.b) / 3.0;
					//////////////////////////////////////////////////////////

					//float2 screenUV = IN.screenPos.xy / IN.screenPos.w;
					float4 col = GetNoiseColor(dist, albedo, colMatr, IN.viewDir);
					colArea.rgb += col.rgb;

					o.Albedo = colArea.rgb;
					o.Alpha = col.a;
				}
			}
		}

		ENDCG
	}

	FallBack "Diffuse"
}
