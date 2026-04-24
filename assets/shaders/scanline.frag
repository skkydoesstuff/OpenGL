#version 460 core
in  vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform sampler2D uNormal;
uniform sampler2D uDepth;
uniform vec2      uOutputSize;

uniform float uScanlineThickness;  // pixels per scanline pair, e.g. 3.0
uniform float uScanlineDarkness;   // 0.0 = no effect, 1.0 = fully black gaps
uniform float uPhosphorStrength;   // RGB channel separation, e.g. 0.3
uniform float uGlowStrength;       // bleed between lines, e.g. 0.4
uniform float uVignetteStrength;   // edge darkening, e.g. 0.4
uniform float uBrightBoost;        // overall brightness, e.g. 1.1

void main() {
    vec2 uv = vTexCoord;

    // ── Subtle barrel distortion to mimic curved CRT glass ──────────────────
    vec2 curved = uv * 2.0 - 1.0;
    vec2 offset = curved.yx * curved.yx * 0.03;
    curved += curved * offset;
    uv = curved * 0.5 + 0.5;

    // Out of bounds after distortion — draw black border
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // ── RGB phosphor separation (chromatic aberration) ───────────────────────
    float sep = uPhosphorStrength / uOutputSize.x;
    float r = texture(uTexture, uv + vec2( sep, 0.0)).r;
    float g = texture(uTexture, uv                  ).g;
    float b = texture(uTexture, uv - vec2( sep, 0.0)).b;
    vec3 color = vec3(r, g, b);

    // ── Scanlines ────────────────────────────────────────────────────────────
    float line      = floor(uv.y * uOutputSize.y / uScanlineThickness);
    float lineFrac  = fract(uv.y * uOutputSize.y / uScanlineThickness);

    // Smooth dark gap at the bottom of each scanline pair
    float scanline  = 1.0 - uScanlineDarkness * smoothstep(0.5, 0.8, lineFrac);

    // ── Phosphor glow — sample adjacent lines and bleed brightness ───────────
    float texelY    = 1.0 / uOutputSize.y;
    vec3 above      = texture(uTexture, uv + vec2(0.0,  texelY * uScanlineThickness)).rgb;
    vec3 below      = texture(uTexture, uv - vec2(0.0,  texelY * uScanlineThickness)).rgb;
    vec3 glow       = (above + below) * 0.5 * uGlowStrength;

    color = color * scanline + glow * (1.0 - scanline * 0.5);

    // ── Vignette ─────────────────────────────────────────────────────────────
    vec2  vigUV    = vTexCoord * 2.0 - 1.0;
    float vignette = 1.0 - dot(vigUV, vigUV) * uVignetteStrength;
    color *= clamp(vignette, 0.0, 1.0);

    // ── Brightness boost and output ──────────────────────────────────────────
    color *= uBrightBoost;

    FragColor = vec4(color, 1.0);
}