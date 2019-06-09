import java.net.*;
import java.io.*;
import java.util.concurrent.*;

public class DateServer {
	
	
	private static void serveRequest(ServerSocket sock, ExecutorService pool) {
		try {
			Socket client = sock.accept();
			PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
			pout.println(new java.util.Date().toString());
			client.close();
		}
		catch(IOException ioe) {
        	throw new UncheckedIOException(ioe);
    	} 
	}

	public static void main(String[] args) {
		try {
			ServerSocket sock = new ServerSocket(6013);
			ExecutorService pool = Executors.newFixedThreadPool(3);


			while (true) {
				Runnable task = () -> {
					DateServer.serveRequest(sock, pool);
				};
				pool.execute(task);
			}
			
		}
		catch (IOException ioe) {
			System.err.println(ioe);
		}
	}
}
