//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// ShadowMapping
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// CreateShadow
//--------------------------------------------------------------//
string ShadowMapping_CreateShadow_Torus : ModelData = ".\\Torus.x";

texture ShadowMap_Tex : RenderColorTarget
<
   float2 RenderTargetDimensions = {2048,2048};
   string Format="D3DFMT_R32F";
   float  ClearDepth=1.000000;
   int    ClearColor=-1;
>;
struct VS_INPUT
{
   float4 mPosition : POSITION;
};

struct VS_OUTPUT
{
   float4 mPosition : POSITION;
   float4 mClipPosition : TEXCOORD1;
};

float4x4 gWorldMatrix : World;
float4x4 gLightViewMatrix;
float4x4 gLightProjectionMatrix : Projection;

float4 gWorldLightPosition
<
   string UIName = "gWorldLightPosition";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 500.00, 500.00, -500.00, 1.00 );

VS_OUTPUT ShadowMapping_CreateShadow_Vertex_Shader_vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   
   float4x4 lightViewMatrix = gLightViewMatrix;
   
   float3 dirZ = -normalize(gWorldLightPosition.xyz);
   float3 up = float3(0, 1, 0);
   float3 dirX = cross(up, dirZ);
   float3 dirY = cross(dirZ, dirX);
   
   lightViewMatrix = float4x4(
      float4(dirX, -dot(gWorldLightPosition.xyz, dirX)),
      float4(dirY, -dot(gWorldLightPosition.xyz, dirY)),
      float4(dirZ, -dot(gWorldLightPosition.xyz, dirZ)),
      float4(0, 0, 0, 1));
   lightViewMatrix = transpose(lightViewMatrix);
      
      Output.mPosition = mul(Input.mPosition, gWorldMatrix);
      Output.mPosition = mul(Output.mPosition, lightViewMatrix);
      Output.mPosition = mul(Output.mPosition, gLightProjectionMatrix);
      
      Output.mClipPosition = Output.mPosition;
      return Output;

}

struct PS_INPUT
{
   float4 mClipPosition : TEXCOORD1;
};

float4x4 ShadowMapping_CreateShadow_Pixel_Shader_gWorldMatrix;
float4x4 ShadowMapping_CreateShadow_Pixel_Shader_gLightViewMatrix;
float4x4 ShadowMapping_CreateShadow_Pixel_Shader_gLightProjectionMatrix;
float4 ShadowMapping_CreateShadow_Pixel_Shader_gWorldLightPosition;

float4 ShadowMapping_CreateShadow_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
   float depth = Input.mClipPosition.z / Input.mClipPosition.w;
   return float4(depth.xxx, 1);
}
//--------------------------------------------------------------//
// Technique Section for ShadowMapping
//--------------------------------------------------------------//
technique ShadowMapping
{
   pass CreateShadow
   <
      string Script = "RenderColorTarget0 = ShadowMap_Tex;"
                      "ClearColor = (255, 255, 255, 255);"
                      "ClearDepth = 1.000000;";
   >
   {
      VertexShader = compile vs_3_0 ShadowMapping_CreateShadow_Vertex_Shader_vs_main();
      PixelShader = compile ps_3_0 ShadowMapping_CreateShadow_Pixel_Shader_ps_main();
   }

}

