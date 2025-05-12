#define MAX_SHADOWS 8

uniform sampler2DShadow ShadowMaps[MAX_SHADOWS];

flat in int NumShadows;

in vec4 ShadowCoord[MAX_SHADOWS];

float calculateShadow()
{
    float shadow = 1.0;
    for(int i = 0; i < NumShadows; ++i) {
        if (ShadowCoord[i].z >= 0.0) {
            float s = textureProj(ShadowMaps[i], ShadowCoord[i]);
            shadow = min(shadow, s);
        }
    }
    return shadow;
}

float calculateShadowPCF()
{
    float shadow = 1.0;
    for(int i = 0; i < NumShadows; ++i) {
        if (ShadowCoord[i].z >= 0.0) {
            
            vec4 sc = ShadowCoord[i];
            bool insideXY = sc.x >= 0.0 && sc.x <= sc.w
                            && sc.y >= 0.0 && sc.y <= sc.w;
            bool insideZ  = sc.z >= 0.0 && sc.z <= sc.w;
            if (!insideXY || !insideZ) {
                // fragment is outside light-i’s view: skip its test
                continue;
            }

            // PCF: sample 4 neighbouring texels
            float sum = 0.0;
            sum += textureProjOffset(ShadowMaps[i], ShadowCoord[i], ivec2(-1, -1));
            sum += textureProjOffset(ShadowMaps[i], ShadowCoord[i], ivec2(-1,  1));
            sum += textureProjOffset(ShadowMaps[i], ShadowCoord[i], ivec2( 1,  1));
            sum += textureProjOffset(ShadowMaps[i], ShadowCoord[i], ivec2( 1, -1));
            float avg = sum * 0.25;
            shadow = min(shadow, avg);
        }
    }
    return shadow;
}