#include "RmlShader.h"

IMPLEMENT_SHADER_TYPE(, FRmlShaderVs, TEXT("/Plugin/RmlPlugin/Private/RmlShader.usf"), TEXT("RmlShader_VS"), SF_Vertex);
IMPLEMENT_SHADER_TYPE(, FRmlShaderPs, TEXT("/Plugin/RmlPlugin/Private/RmlShader.usf"), TEXT("RmlShader_PS"), SF_Pixel);
IMPLEMENT_SHADER_TYPE(, FRmlShaderPsNoTex, TEXT("/Plugin/RmlPlugin/Private/RmlShader.usf"), TEXT("RmlShader_PSNoTex"), SF_Pixel);
