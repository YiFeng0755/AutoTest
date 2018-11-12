package YoumeUtil;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MockServer {
    private int port = 10009;
    private ServerSocket serverSocket;
    private ExecutorService executorService;
    private final int POOL_SIZE = 20;

    public MockServer() throws IOException {
        serverSocket = new ServerSocket(port);
        executorService = Executors.newFixedThreadPool(Runtime.getRuntime()
                .availableProcessors() * POOL_SIZE);
    }

    public void service() {
        while (true) {
            Socket socket = null;
            try {

                socket = serverSocket.accept();
                executorService.execute(new Handler(socket));

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) throws IOException {
        new MockServer().service();
    }

}

class Handler implements Runnable {
    private Socket socket = null;

    public Handler(Socket socket) {
        this.socket = socket;
    }

    private BufferedReader getReader(Socket socket) throws IOException {
        InputStream in = socket.getInputStream();
        return new BufferedReader(new InputStreamReader(in));
    }

    public void run() {
        BufferedReader br = null;
        PrintWriter out = null;

        System.out.println("New connection accepted " + socket.getInetAddress()
                + ":" + socket.getPort());

        try {
            br = getReader(socket);

            String recv = null;
            recv = br.readLine();

            System.out.println(recv);

            out = new PrintWriter(socket.getOutputStream());
            out.write("Hello Client\r\n");
            out.flush();

        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (br != null) {
                    br.close();
                }
                if (out != null) {
                    out.close();
                }
                if (socket != null)
                    socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}