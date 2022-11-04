
uniform sampler2D u_texture;
uniform float u_time;

void main(){

	vec2 pos = gl_TexCoord[0].xy;

	vec2 offset = cos(vec2(pos.y * 50.0, pos.x * 20.0) + u_time) * 0.006;

	gl_FragColor = texture2D(u_texture, pos + offset);
}