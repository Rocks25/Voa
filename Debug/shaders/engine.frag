uniform sampler2D tex;

in vec4 color;

out vec4 outputF;
 
void main()
{
    vec4 color_base = texture2D(tex,gl_TexCoord[0].st);
    outputF = vec4(color_base.r,color_base.g,color_base.b,color.a);   
//    gl_FragColor = vec4(color_base.rgb,al);
}


