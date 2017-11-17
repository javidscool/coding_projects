#version 120

uniform sampler2D normaltexture;
uniform sampler2D depthtexture;
uniform sampler2D positionstexture;
uniform mat4 projection;
varying vec2 texcoord;

void main(){

vec3 tex =texture2D(normaltexture, texcoord).rgb;

//we initialize the clear color to black for the deferred g-buffer passes.  just check the length of 
//the vector we look up.  discard if we didn't render a normal.  this allows us to see the skybox
if(length(tex) < .01)discard;

//todo: optimize somehow?
//get world space positions from the g-buffer
vec4 pos = vec4(texture2D(positionstexture, texcoord).rgb, 1.0);

vec3 normallookup = (normalize(tex));

//dot product with some directional light
vec3 color = vec3(abs(dot(
		normallookup, vec3(0,0,1)
		)));


gl_FragColor = vec4(color, 1.0);

}
