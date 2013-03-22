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
      float imgpos=(spread*(i-samples/2.0))/(samples/2.0);
      float weight = .16-abs(i/(samples/2.0*10)-.1);
      sum += texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y+imgpos*blur)) * weight;
      total+=weight;
   }
//   sum=sum/samples+sum/total;
   sum.a=min(color.a,al);
   gl_FragColor = sum;
}  