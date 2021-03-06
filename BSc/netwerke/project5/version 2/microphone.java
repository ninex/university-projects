import java.net.*;
import java.io.*;
import javax.sound.sampled.*;

public class microphone extends Thread implements Runnable
{
	Socket client = null;
	AudioFormat format;
	int framesize;
	TargetDataLine line = null;
	boolean notready;
	public microphone(Socket client)
	{
		this.client = client;
		notready = true;
	}
	
    // this function is called when you start to execute the thread
	public void run()
	{
// 		try
// 		{
// 			sleep(150);
// 		}catch(Exception e){}
		setupAudio();
		ObjectOutputStream in = null;
		AudioInputStream au_in = null;
	
		// Two messages needed when communicating between client and server (thread)
		String msg_in = null;
		String host=null;
		int frameSizeInBytes = format.getFrameSize();
		int bufferLengthInFrames = line.getBufferSize() / 8;
		int bufferLengthInBytes = bufferLengthInFrames * frameSizeInBytes;
		byte[] data = new byte[bufferLengthInBytes];
		int numBytesRead;
        // try and create input streams on which to pass and recieve
		try
		{
			in = new ObjectOutputStream(client.getOutputStream());
			//au_in = new AudioInputStream(in,format,framesize);
		}catch (Exception e){}
		notready = false;
		// If a client has been connected to the socket the proceed
		while(client.isConnected())
		{
			//attempt to recive the message from the input stream
			try {
				//host = (String)(in.readObject());
				//msg_in = (String)(in.readObject());
				//a
				if (line.available() == 0)
				{
					numBytesRead = -1;
				}else
				{
					numBytesRead = line.read(data,0,bufferLengthInBytes);
				}
				if (numBytesRead != -1)
				{
					in.write(data,0,numBytesRead);
					
				}else
				{
					//System.out.println("Error writing to stream");
				}
			}catch (Exception e){e.printStackTrace();}
			
			
                        
            // simply to display that the conection was closed
			if (client.isClosed())
			{
				System.out.println("Connection was closed");
			}
		}
		System.out.println("Connection was closed");
	}
	
	public void setupAudio ()
	{
	
	
		if (AudioSystem.getMixer(null) == null)
		{
			System.out.println("Audio system unavailable\n");
		}
		AudioInputStream inp = null;
		// define the required attributes for our line, 
           	 // and make sure a compatible line is supported.
		
		AudioFormat.Encoding encoding = AudioFormat.Encoding.PCM_SIGNED;
		float rate = 44100;
		int sampleSize = 16;
		int channels = 1;
		boolean bigEndian = true;
		framesize = (sampleSize/8)*channels;
		DataLine.Info info;
		
		
		format = new AudioFormat(encoding,rate,sampleSize,channels,framesize,rate,bigEndian);
		info = new DataLine.Info(TargetDataLine.class, format);
				
		if (!AudioSystem.isLineSupported(info)) {
			System.out.println("Line matching " + info + " not supported.");
			System.exit(1);
		}else
		{
			System.out.println("Line matching " + info + " supported.");
			
		}
		boolean ready = true;
		while (ready)
		{
			try {
				line = (TargetDataLine) AudioSystem.getLine(info);
				line.open(format, line.getBufferSize());
				System.out.println("Line opened");
				ready = false;
			} catch (LineUnavailableException ex) 
			{ /*
				System.out.println("Unable to open the line: " + ex);
				System.exit(1);*/
			}
		}
		// play back the captured audio data
		ByteArrayOutputStream out = new ByteArrayOutputStream();
		int frameSizeInBytes = format.getFrameSize();
		int bufferLengthInFrames = line.getBufferSize() / 8;
		int bufferLengthInBytes = bufferLengthInFrames * frameSizeInBytes;
		byte[] data = new byte[bufferLengthInBytes];
		int numBytesRead;
		
		line.start();
	/*
		while (true) {
			if((numBytesRead = line.write(data, 0, bufferLengthInBytes)) == -1) {
			System.out.println("No data written");
			break;
			}
			out.write(data, 0, numBytesRead);
		}
	
		// we reached the end of the stream.  stop and close the line.
		System.out.println("Line closed\n");
		line.stop();
		line.close();
		line = null;
		/*Audio au = new Audio();
		au.init();
		au.exit();*/
	}/*
	public static void main(String[] args)
	{
		try
		{
			ServerSocket theserver = new ServerSocket(3200);
			Socket theclient = theserver.accept();
			(new msgThread2(theclient)).start();
		}catch (Exception e){e.printStackTrace();}
		
	}*/
	public boolean mustWait()
	{
		return notready;
	}
}
