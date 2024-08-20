import java.io.*;
import java.net.*;

public class Server{
	int port;
	DataInputStream dis=null;
	DataOutputStream dos=null;
	ServerSocket ss;
	Server(int port){
		this.ss=new ServerSocket(port);
		Socket s=ss.accept();
		DataInputStream dis=new DataInputStream(s.getInputStream());
		this.read();
	}
	void read(){
		String str=(String)dis.readUTF();
		System.out.println("Message from client : "+str);
		if(str.equals("Over")){
			this.close();
		}
	}
	void close(ServeSocket ss){
		ss.close();
	}
	public static void main(String[] args){
		Server myserver=new Server(5000);	
	}
}	

