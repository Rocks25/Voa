uniform sampler2D tex;
uniform sampler2D alpha;
uniform float blur;

varying vec4 color;

void main()
{
   float al = texture2D(alpha, gl_TexCoord[0].st).r;
   //Gaussian Blur filter
   vec4 sum = vec4(0.0);
 
   // blur in y (vertical)
   // take nine samples, with the distance blurX between them
   int samples=9;
   float total = 0.0;
   float spread = 4.0;
   for(int i=0;i<samples;i++)
   {
      float imgposn = spread*(i-samples/2.0);
      float imgposd = (samples/2.0);
      float imgpos=imgposn/imgposd;
      float weight = .16-abs(i/(samples/2.0*10)-.1);
      sum += texture2D(tex, vec2(gl_TexCoord[0].x + imgpos*blur, gl_TexCoord[0].y)) * weight;
      total+=weight*2;
   }
   sum=sum/samples+sum/total;
   //sum.a=min(color.a,al);
   gl_FragColor = sum;
}  