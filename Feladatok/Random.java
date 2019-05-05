public class PolarGenerator
{
    boolean notStored = true;
    double storedValue;
    
    public PolarGenerator()
    {
        notStored = true;
    }
    
    public double next()
    {
        if(notStored)
        {
            double u1,u2,v1,v2,w;
            do
            {
            
                u1 = Math.random();
                u2 = Math.random();
                v1 = 2 * u1 - 1;
                v2 = 2 * u2 -1;
                w = v1*v1 + v2*v2;
            
            } while(w>1);
            
            double r = Math.sqrt((-2*Math.log(w))/w);
            storedValue = r* v2;
            notStored = !notStored;
            return r * v1;
        }
        else
        {
            notStored = !notStored;
            return storedValue;
        }
    }
    
    public static void main(String[] args)
    {
        PolarGenerator g = new PolarGenerator();
        for(int i=0; i<10; i++)
        {
            System.out.println(g.next());
        }
    }
}
