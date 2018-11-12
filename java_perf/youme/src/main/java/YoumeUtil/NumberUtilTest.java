package YoumeUtil;

public class NumberUtilTest {
    public static String byte2Hex(byte[] buf) 
    {
        StringBuffer strbuf = new StringBuffer();
        strbuf.append("{");
        for (byte b : buf) 
        {
            if (b == 0) 
            {
                strbuf.append("00");
            } 
            else if (b == -1) 
            {
                strbuf.append("FF");
            } 
            else 
            {
                String str = Integer.toHexString(b).toUpperCase();
                // sb.append(a);
                if (str.length() == 8) 
                {
                    str = str.substring(6, 8);
                } 
                else if (str.length() < 2) 
                {
                    str = "0" + str;
                }
                strbuf.append(str);
            }
            strbuf.append(" ");
        }
        strbuf.append("}");
        return strbuf.toString();
    }    

    public static byte[] longToBytes(long n) {
        byte[] b = new byte[8];
        b[7] = (byte) (n & 0xff);
        b[6] = (byte) (n >> 8  & 0xff);
        b[5] = (byte) (n >> 16 & 0xff);
        b[4] = (byte) (n >> 24 & 0xff);
        b[3] = (byte) (n >> 32 & 0xff);
        b[2] = (byte) (n >> 40 & 0xff);
        b[1] = (byte) (n >> 48 & 0xff);
        b[0] = (byte) (n >> 56 & 0xff);
        return b;
    }

    public static long bytesToLong( byte[] array )
    {
        return ((((long) array[ 0] & 0xff) << 56)
              | (((long) array[ 1] & 0xff) << 48)
              | (((long) array[ 2] & 0xff) << 40)
              | (((long) array[ 3] & 0xff) << 32)
              | (((long) array[ 4] & 0xff) << 24)
              | (((long) array[ 5] & 0xff) << 16)
              | (((long) array[ 6] & 0xff) << 8) 
              | (((long) array[ 7] & 0xff) ));        
    }
    
    public static int bytesToInt(byte b[]) {
        return    b[3] & 0xff 
               | (b[2] & 0xff) << 8 
               | (b[1] & 0xff) << 16
               | (b[0] & 0xff) << 24;
    }

    public static long bytesToUint(byte[] array) {  
        return ((long) (array[3] & 0xff))  
             | ((long) (array[2] & 0xff)) << 8  
             | ((long) (array[1] & 0xff)) << 16  
             | ((long) (array[0] & 0xff)) << 24;  
    }

    public static byte[] uintToBytes( long n )
    {
        byte[] b = new byte[4];
        b[3] = (byte) (n & 0xff);
        b[2] = (byte) (n >> 8 & 0xff);
        b[1] = (byte) (n >> 16 & 0xff);
        b[0] = (byte) (n >> 24 & 0xff);
        
        return b;
    }
    

    public static byte[] shortToBytes(short n) {
        byte[] b = new byte[2];
        b[1] = (byte) ( n       & 0xff);
        b[0] = (byte) ((n >> 8) & 0xff);
        return b;
    }
    
    public static short bytesToShort(byte[] b){
        return (short)( b[1] & 0xff
                      |(b[0] & 0xff) << 8 ); 
    }
    
    static void testShortConvert(){
        System.out.println("=================== short convert =============");
        System.out.println("byte2Hex(shortToBytes((short)0x11f2))"+byte2Hex(shortToBytes((short)0x11f2)));        
        System.out.print("println 0x11f2:");
        System.out.println((short)0x11f2);        
        System.out.println("byte2Hex(shortToBytes((short)0xf1f2))"+byte2Hex(shortToBytes((short)0xf1f2)));        
        System.out.print("println 0xf1f2:");
        System.out.println((short)0xf1f2);            
        System.out.print("println bytesToShort(shortToBytes((short)0x11f2)):");
        System.out.println((short)bytesToShort(shortToBytes((short)0x11f2)));            
        System.out.print("println bytesToShort(shortToBytes((short)0xf1f2)):");
        System.out.println((short)bytesToShort(shortToBytes((short)0xf1f2)));        
    }
    

    public static byte[] ushortToBytes(int n) {
        byte[] b = new byte[2];
        b[1] = (byte) (n & 0xff);
        b[0] = (byte) (n >> 8 & 0xff);
        return b;
    }
    

    public static int bytesToUshort(byte b[]) {
        return    b[1] & 0xff 
               | (b[0] & 0xff) << 8;
    }

    static void testUshortConvert(){
        System.out.println("=================== Ushort convert =============");
        System.out.println("byte2Hex(ushortToBytes(0x11f2))"+byte2Hex(ushortToBytes(0x11f2)));        
        System.out.print("println 0x11f2:");
        System.out.println(0x11f2);        
        System.out.println("byte2Hex(ushortToBytes(0xf1f2))"+byte2Hex(ushortToBytes(0xf1f2)));        
        System.out.print("println 0xf1f2:");
        System.out.println(0xf1f2);            
        System.out.print("println bytesToUshort(ushortToBytes(0x11f2)):");
        System.out.println(bytesToUshort(ushortToBytes(0x11f2)));            
        System.out.print("println bytesToUshort(ushortToBytes(0xf1f2)):");
        System.out.println(bytesToUshort(ushortToBytes(0xf1f2)));        
    }
    
    public static byte[] ubyteToBytes( int n ){
        byte[] b = new byte[1];
        b[0] = (byte) (n & 0xff);
        return b;
    }

    public static int bytesToUbyte( byte[] array ){            
        return array[0] & 0xff;
    }    

    static void testUbyteConvert(){
        System.out.println("=================== Ubyte convert =============");
        System.out.println("byte2Hex(ubyteToBytes(0x1112))"+byte2Hex(ubyteToBytes(0x1112)));        
        System.out.print("println 0x1112:");
        System.out.println(0x1112);        
        System.out.println("byte2Hex(ubyteToBytes(0xf2))"+byte2Hex(ubyteToBytes(0xf2)));        
        System.out.print("println 0xf2:");
        System.out.println(0xf2);            
        System.out.print("println bytesToUbyte(ubyteToBytes(0x1112)):");
        System.out.println(bytesToUbyte(ubyteToBytes(0x1112)));            
        System.out.print("println bytesToUbyte(ubyteToBytes(0xf1f2)):");
        System.out.println(bytesToUbyte(ubyteToBytes(0xf1f2)));        
    }
    
    
    /**
     * @param args
     */
    public static void main(String[] args) {
        // TODO Auto-generated method stub        
        byte[] array = new byte[4];
        array[3] = (byte) 0xF4;
        array[2] = 0x13;
        array[1] = 0x12;
        array[0] = 0x11;
        
        System.out.println("=================== Integer bytes =============");
        
        System.out.println("the bytes is:"+byte2Hex(array) );
        System.out.print("println bytesToInt :");
        System.out.println( bytesToInt(array));
        System.out.printf("printf bytesToInt :%X\n", bytesToInt(array));
        
        System.out.println("=================== long bytes =============");
        byte[] longBytes = new byte[8];
        
        longBytes[7] = (byte) 0xf7;
        longBytes[6] = (byte) 0x16;
        longBytes[5] = (byte) 0xf5;
        longBytes[4] = (byte) 0x14;
        longBytes[3] = (byte) 0xf3;
        longBytes[2] = (byte) 0x12;
        longBytes[1] = (byte) 0xf1;
        longBytes[0] = (byte) 0x10;
        

        System.out.println( "the bytes is:"+byte2Hex(longBytes) );
        System.out.printf("printf bytesToLong:%X\n",bytesToLong(longBytes));
        
        System.out.println("=================byte to long ================");
        
        byte b = (byte)0xf1;
        System.out.print("Println the byte:");
        System.out.println(b);
        System.out.printf("Printf the byte:%X\n",b);
        long l = b;
        System.out.print("Println byte to long:");
        System.out.println(l);
        System.out.printf("printf byte to long:%X\n",l);
        
        System.out.println("================= uint Bytes ================");
        
        byte[] uint = new byte[4];
        uint[3] = (byte) 0xf3;
        uint[2] = (byte) 0x12;
        uint[1] = (byte) 0xf1;
        uint[0] = (byte) 0xFF;
        
        System.out.println( "the bytes is:"+byte2Hex(uint) );
        System.out.printf("printf bytesToUint:%X\n",bytesToUint(uint));
        System.out.print("Println bytesToUint:");
        System.out.println(bytesToUint(uint));
        System.out.println("byte2Hex(uintToBytes(0x11f2f3f4f5f6f7f8l)):"+byte2Hex(uintToBytes(0x11f2f3f4f5f6f7f8l)));
        
        System.out.println("===============Long Integer==============");        
        System.out.print("println 0x11f2f3f4f5f6f7f8l:");
        System.out.println(0x11f2f3f4f5f6f7f8l);        
        System.out.printf("Printf 0x11f2f3f4f5f6f7f8l:%X\n",0x11f2f3f4f5f6f7f8l);
        System.out.println("println byte2Hex(longToBytes(0x11f2f3f4f5f6f7f8l))"+byte2Hex(longToBytes(0x11f2f3f4f5f6f7f8l)));
        // 注意，下面的这行，并不能获得正确的uint。
        System.out.printf("printf bytesToUint(longToBytes(0x11f2f3f4f5f6f7f8l):%X\n",bytesToUint(longToBytes(0x11f2f3f4f5f6f7f8l)));
        
        System.out.println("===============bytesToLong(longToBytes())==============");
        System.out.println(bytesToLong(longToBytes(0x11f2f3f4f5f6f7f8l)));
        System.out.printf("%X\n",bytesToLong(longToBytes(0x11f2f3f4f5f6f7f8l)));
        
        testShortConvert();
        testUshortConvert();
        testUbyteConvert();
    }
}
