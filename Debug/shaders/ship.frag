uniform sampler2D tex;
uniform sampler2D alpha;

in vec4 color;
out vec4 outputF;
 
void main()
{
    vec4 color_base = texture2D(tex,gl_TexCoord[0].st);
    float al = texture2D(alpha, gl_TexCoord[0].st).r;
//    outputF = vec4(color_base.r,color_base.g,color_base.b,al);   
    gl_FragColor = vec4(color_base.rgb,min(al,color.a));
}


