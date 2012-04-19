import java.net.*;
import java.io.*;
import javax.sound.sampled.*;

public class microphone extends Thread implements Runnable
{
	Socket client = null;
	AudioFormat format;
	int framesize;
	TargetDataLine line = null;
	SourceDataLine line2 = null;
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
		System.out.println("[Microphone]Audio initialized");
		ObjectOutputStream out = null;
// 		ObjectInputStream in = null;
// 		AudioInputStream au_in = null;
	
		// Two messages needed when communicating between client and server (thread)
		/*String msg_in = null;
		String host=null;*/
		int frameSizeInBytes = format.getFrameSize();
		int bufferLengthInFrames = line.getBufferSize() / 8;
		int bufferLengthInBytes = bufferLengthInFrames * frameSizeInBytes;
		byte[] data = new byte[bufferLengthInBytes];
		byte[] data2 = new byte[bufferLengthInBytes];
		int numBytesRead, bytesRead;
        // try and create input streams on which to pass and recieve
		try
		{
			out = new ObjectOutputStream(client.getOutputStream());
			//in = new ObjectInputStream(client2.getInputStream());
			//au_in = new AudioInputStream(in,format,framesize);
		}catch (Exception e){e.printStackTrace();}
		notready = false;
		//System.out.println("Streams initialized. out = "+out+" in = "+in+" size:"+data.length);
		// If a client has been connected to the socket the proceed
		while (client.isConnected())
		{
			//System.out.println("in loop");
			//attempt to recive the message from the input stream
			try {
				//host = (String)(in.readObject());
				//msg_in = (String)(in.readObject());
				//a
				//System.out.println("in loop3"+line.available());
				//line2.stop();
				//line.start();
				//if (line.available() != 0)
				//{
				
					numBytesRead = line.read(data,0,bufferLengthInBytes);
				
					//System.out.println("in loop3");
					if (numBytesRead != -1)
					{
						System.out.println("Wrote "+numBytesRead+" bytes to stream");
						out.write(data,0,numBytesRead);
					}else
					{
						System.out.println("Error writing to stream");
					}
				//}//line.stop();
				/*line2.start();
				if (in.available() >= 2)
				{
				
					bytesRead = in.read(data2,0,2);
					if (bytesRead != -1)
					{
						System.out.println("Read "+bytesRead+" bytes from stream");
						line2.write(data2,0,bytesRead);
						
					}else
					{
						System.out.println("Read "+bytesRead+" bytes from stream");
					}
				}*/
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
		DataLine.Info info,info2;
		
		
		format = new AudioFormat(encoding,rate,sampleSize,channels,framesize,rate,bigEndian);
		info = new DataLine.Info(TargetDataLine.class, format);
		//info2 = new DataLine.Info(SourceDataLine.class, format);
		if (!AudioSystem.isLineSupported(info)) {
			System.out.println("Line matching " + info + " not supported.");
			System.exit(1);
		}else
		{
			System.out.println("Line matching " + info + " supported.");
			
		}
		/*if (!AudioSystem.isLineSupported(info2)) {
			System.out.println("Line matching " + info2 + " not supported.");
			System.exit(1);
		}else
		{
			System.out.println("Line matching " + info2 + " supported.");
			
		}*/
		boolean ready = true;
		while (ready)
		{
			try {
				line = (TargetDataLine) AudioSystem.getLine(info);
				line.open(format, line.getBufferSize());
				System.out.println("Line opened");
				ready = false;
				line.start();
		System.out.println("Line1 started");
			} catch (LineUnavailableException ex) {}
		}
		/*ready = true;
		while (ready)
		{
			try
			{
				line2 = (SourceDataLine) AudioSystem.getLine(info2);
				line2.open(format, line2.getBufferSize());
				System.out.println("Line2 opened");
				ready = false;
			} catch (LineUnavailableException ex) {}
		}
		Line ls[] = new Line[2];
		ls[0] = line;
		ls[1] = line2;
		Mixer.Info ifd[] = AudioSystem.getMixerInfo();
		for (int i=0;i<ifd.length;i++)
		System.out.println("DIT IS "+(AudioSystem.getMixer(ifd[i])).isSynchronizationSupported(ls,false));*/
		// play back the captured audio data
		/*ByteArrayOutputStream out = new ByteArrayOutputStream();
		int frameSizeInBytes = format.getFrameSize();
		int bufferLengthInFrames = line.getBufferSize() / 8;
		int bufferLengthInBytes = bufferLengthInFrames * frameSizeInBytes;
		byte[] data = new byte[bufferLengthInBytes];
		int numBytesRead;*/
		
		
		//line2.start();
		System.out.println("Line2 started");
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
