import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

public class EchoClient {
	public static void main(String[] args) {
		try {
			Socket sock = new Socket ("127.0.0.1",6017);
			InputStream in = sock.getInputStream();
			BufferedInputStream bin = new BufferedInputStream(in);
			PrintWriter pout = new PrintWriter(sock.getOutputStream(), true);
			Scanner reader = new Scanner(System.in);  
			System.out.println("Enter a message and wait for server response:");
			String s = reader.nextLine(); 
			pout.println(s);
			reader.close();
			int byteRead;
			byteRead = bin.read();
			while (byteRead != 0) {
				System.out.print((char)byteRead);
				byteRead = bin.read();			
			}
			sock.close();
		}
		catch (IOException ioe) {
			System.err.println(ioe);
		}
	}
}
