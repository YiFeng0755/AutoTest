package YoumeUtil;

import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.interfaces.RSAPrivateKey;
import java.security.interfaces.RSAPublicKey;
import javax.crypto.Cipher;

import java.util.Base64;
 
/**
 * RSA������
 *
 */
public class CryptUtil {
	
	private final String appkey = "YOUMEF405C62A2744F22231463EAFB2AFB62749C7F93D";
	private final String appSecret = "vxMKLHmmzuoCtUZavCIRMv/e4a9uQ3aoitTVGF9hAtzTVpii1kYXPlXCjcJ7gUrr7rbgt7LKvpEbhQcSrr0jpo9sFt18uNdw5tFiYaD2+Bzt0Vukvktfqoi9HjJuzMVl+gaUFO0jGsr6i14+0RAQZl2Oa3q1xTzpPvphzdpgMzkBAAE=";
	
    public static void main(String[] args) {
        try {
        	CryptUtil encrypt = new CryptUtil();
            String encryptText = "12345678";
            KeyPairGenerator keyPairGen = KeyPairGenerator.getInstance("RSA");
            keyPairGen.initialize(1024);
            KeyPair keyPair = keyPairGen.generateKeyPair();
            // Generate keys
            RSAPrivateKey privateKey = (RSAPrivateKey) keyPair.getPrivate(); // ˽Կ
            RSAPublicKey publicKey = (RSAPublicKey) keyPair.getPublic(); // ��Կ
            byte[] e = encrypt.encrypt(publicKey, encryptText.getBytes());
            byte[] de = encrypt.decrypt(privateKey, e);
            System.out.println(encrypt.bytesToString(e));
            System.out.println();
 
            System.out.println(encrypt.bytesToString(de));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * byte����תΪstring
     *
     * @param encrytpByte
     * @return
     */
    protected String bytesToString(byte[] encrytpByte) {
        String result = "";
        for (Byte bytes : encrytpByte) {
            result += (char) bytes.intValue();
        }
        return result;
    }
 
    /**
     * ���ܷ���
     *
     * @param publicKey
     * @param obj
     * @return
     */
    protected byte[] encrypt(RSAPublicKey publicKey, byte[] obj) {
        if (publicKey != null) {
            try {
                Cipher cipher = Cipher.getInstance("RSA");
                cipher.init(Cipher.ENCRYPT_MODE, publicKey);
                return cipher.doFinal(obj);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return null;
    }
 
    /**
     * ���ܷ���
     *
     * @param privateKey
     * @param obj
     * @return
     */
    protected byte[] decrypt(RSAPrivateKey privateKey, byte[] obj) {
        if (privateKey != null) {
            try {
                Cipher cipher = Cipher.getInstance("RSA");
                cipher.init(Cipher.DECRYPT_MODE, privateKey);
                return cipher.doFinal(obj);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return null;
    }
}