#version 460

in vec2 TexCoord;

uniform sampler2D HdrTex;
uniform sampler2D BlurTex1;
uniform sampler2D BlurTex2;


layout (location = 0) out vec4 FragColor;

//------------ TONE MAPPING PARAMS ------------//

uniform float AveLum;
uniform float LumThresh;
uniform float Exposure = 0.15;
uniform float White = 0.928;
uniform bool DoToneMap = true;

uniform mat3 rgb2xyz = mat3(
    0.4142564, 0.2126729, 0.0193339,
    0.3575761, 0.7151522, 0.1191920,
    0.1804375, 0.0721750, 0.9503041
);

uniform mat3 xyz2rgb = mat3(
    3.2404542, -0.9692660,  0.0556434,
   -1.5371385,  1.8760108, -0.2040259,
   -0.4985314,  0.0415560,  1.0572252
);

//------------ BLUR PARAMETERS ------------//

const int KERNEL_SIZE = 10;
uniform float PixOffset[KERNEL_SIZE];
uniform float Weight[KERNEL_SIZE];

//------------ CONTROL ------------//

uniform int Pass;
float Gamma = 1.0;

//------------ UTILITY FUNCTIONS ------------//

// calculates perceived brightness using luminance weights
float luminance(vec3 colour) {
    return colour.r * 0.2126 + colour.g * 0.7152 + colour.b * 0.0722;
}

//------------ PASS FUNCTIONS ------------//

vec4 ComputeLogLuminance() {
    vec3 hdrColor = texture(HdrTex, TexCoord).rgb;
    float lum = 1.0; //dot(hdrColor, vec3(0.2126, 0.7152, 0.0722));
    return vec4(log(lum + 0.00001), 0.0, 0.0, 1.0);
}

// isolates bright pixels from HDR image based on luminance threshold
vec4 ThresholdBrightAreas() {
    vec4 hdrVal = texture(HdrTex, TexCoord);
    return luminance(hdrVal.rgb) > LumThresh ? hdrVal : vec4(0.0);
}

// applies vertical blur using BlurTex1
vec4 VerticalBlur() {
    float dy = 1.0 / textureSize(BlurTex1, 0).y;
    vec4 sum = texture(BlurTex1, TexCoord) * Weight[0];

    for (int i = 0; i < KERNEL_SIZE; i++) {
        float offset = PixOffset[i] * dy;
        sum += texture(BlurTex1, TexCoord + vec2(0.0,  offset)) * Weight[i];
        sum += texture(BlurTex1, TexCoord - vec2(0.0,  offset)) * Weight[i];
    }
    return sum;
}

// applies horizontal blur using BlurTex2
vec4 HorizontalBlur() {
    float dx = 1.0 / textureSize(BlurTex2, 0).x;
    vec4 sum = texture(BlurTex2, TexCoord) * Weight[0];

    for (int i = 0; i < KERNEL_SIZE; i++) {
        float offset = PixOffset[i] * dx;
        sum += texture(BlurTex2, TexCoord + vec2( offset, 0.0)) * Weight[i];
        sum += texture(BlurTex2, TexCoord - vec2( offset, 0.0)) * Weight[i];
    }
    return sum;
}

// applies tone mapping to HDR input and combine with blurred bloom texture
vec4 ApplyToneMapping() {
    vec4 hdrColor = texture(HdrTex, TexCoord);

    // converts RGB to XYZ
    vec3 xyz = rgb2xyz * hdrColor.rgb;
    float sum = xyz.x + xyz.y + xyz.z;
    vec3 xyY = vec3(xyz.x / sum, xyz.y / sum, xyz.y);

    // tone mapping luminance
    float L = (Exposure * xyY.z) / AveLum;
    L = (L * (1.0 + L / (White * White))) / (1.0 + L);

    // rebuilds XYZ from tone-mapped luminance
    xyz.x = (L * xyY.x) / xyY.y;
    xyz.y = L;
    xyz.z = (L * (1.0 - xyY.x - xyY.y)) / xyY.y;

    // converts back to RGB
    vec3 toneMappedRGB = xyz2rgb * xyz;
    vec4 toneMapColor = vec4(toneMappedRGB, hdrColor.a);

    // adds blurred bloom texture
    vec4 bloom = texture(BlurTex1, TexCoord);
    //return toneMapColor + bloom;
    return vec4(toneMapColor.rgb + bloom.rgb, toneMapColor.a);
}

//------------ MAIN ------------//

void main() {
    switch (Pass) {
        case 1:
            FragColor = ComputeLogLuminance();
            break;
        case 2:
            FragColor = ThresholdBrightAreas();
            break;
        case 3:
            FragColor = VerticalBlur();
            break;
        case 4:
            FragColor = HorizontalBlur();
            break;
        case 5:
            vec4 color = ApplyToneMapping();
            color.rgb = pow(color.rgb, vec3(1.0 / Gamma));
            FragColor = color;
            break;
        default:
            FragColor = vec4(0.0);
    }
}
