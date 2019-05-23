import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class EchoServer {
	public static void main(String[] args) {
		try {
			ServerSocket sock = new ServerSocket(6017);
			Socket client = sock.accept();
			InputStream in = client.getInputStream();
			BufferedInputStream bin = new BufferedInputStream(in);
			PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
			int byteRead;
			while ((byteRead = bin.read()) != -1) {	
				pout.println((char)byteRead);
			}
			
			client.close();
			sock.close();
			
		}
		catch (IOException ioe) {
			System.err.println(ioe);
		}
	}
}
