public class exor {
    
    public exor(String keyString,
            java.io.InputStream inputStream,
            java.io.OutputStream outputStream)
            throws java.io.IOException {
        
        byte [] kulcs = keyString.getBytes();
        byte [] buffer = new byte[256];
        int keyIndex = 0;
        int readBytes = 0;

        while((readBytes =
                inputStream.read(buffer)) != -1) {
            
            for(int i=0; i<readBytes; ++i) {
                
                buffer[i] = (byte)(buffer[i] ^ kulcs[keyIndex]);
                keyIndex = (keyIndex+1) % kulcs.length;
                
            }
            
            outputStream.write(buffer, 0, readBytes);
            
        }
        
    }
    
    public static void main(String[] args) {
        
        try {
            
            new exor(args[0], System.in, System.out);
            
        } catch(java.io.IOException e) {
            
            e.printStackTrace();
            
        }
        
    }
    
}