uniform sampler2D tex;
uniform sampler2D alpha;

varying vec4 color;
 
void main()
{
    vec4 color_base = texture2D(tex,gl_TexCoord[0].st);
    float al = texture2D(alpha, gl_TexCoord[0].st).r;
    float tmpcolor = (color_base.r+color_base.g+color_base.b)/3.0;
    gl_FragColor = vec4(color_base.rgb,min(al,color.a));
//    gl_FragColor = vec4(color_base.rgb*contrast+brightness,min(al,color.a));
}


