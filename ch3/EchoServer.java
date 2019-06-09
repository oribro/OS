import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.*;
import java.io.UncheckedIOException;

public class EchoServer {

	private static void serveRequest(Socket client) {
		try {
			InputStream in = client.getInputStream();
			BufferedInputStream bin = new BufferedInputStream(in);
			PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
			int byteRead;
			while ((byteRead = bin.read()) != -1) {	
				pout.println((char)byteRead);
			}
			
			client.close();
		}
		catch(IOException ioe) {
        	throw new UncheckedIOException(ioe);
    	} 
	}


	public static void main(String[] args) {
		try {

			ServerSocket sock = new ServerSocket(6017);

			while (true) {
				Socket client = sock.accept();

				Runnable request = () -> {
					EchoServer.serveRequest(client);
				};

				Thread t = new Thread(request);
				t.start();
			}
		}
		catch (IOException ioe) {
			System.err.println(ioe);
		}
	}
}
