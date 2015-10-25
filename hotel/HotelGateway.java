import java.io.*;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.lang.Exception;
import java.lang.Runnable;
import java.lang.String;
import java.lang.Thread;
import java.lang.reflect.Array;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.file.Path;
import java.rmi.Naming;
import java.util.ArrayList;

public class HotelGateway {

    public static int PORTNUM;
    final static String HELP_MESSAGE = "For help: h\nTo list available rooms: l\nTo book a room: b <type> <name>\nTo list guests: g";

    public static String[] openFile(String path) throws IOException {
        FileReader fr = new FileReader(path);
        BufferedReader textReader = new BufferedReader(fr);

        ArrayList<String> textData = new ArrayList<String>();
        String sCurrentLine;
        while ((sCurrentLine = textReader.readLine()) != null) {
           textData.add(sCurrentLine);
        }
        textReader.close();
        return textData.toArray(new String[textData.size()]);
    }

    //parse the config file and get the portnum
    public static int getPortNum() {
        int portnum = -1;
        try {
            String[] config = openFile("../config.ini");
            boolean portNumFound = false;
            int i = 0;
            while (!portNumFound) {
                if (config[i].equals("[hotelgw]")) {
                    String str = config[i + 2];
                    portnum = Integer.parseInt(str.split(":")[1].trim());
                    portNumFound = true;
                }
                i++;
            }
        } catch (IOException io) {
            io.printStackTrace();
        }
        return portnum;
    }

    public static void main(String args[]) {
        try {
            PORTNUM = getPortNum();
            ServerSocket serverSocket = new ServerSocket(PORTNUM);
            while (true) {
                final Socket clientSocket = serverSocket.accept();
                final HotelInterface hotel = (HotelInterface) Naming.lookup("rmi://localhost/HotelService");
                Runnable task = new Runnable() {
                    public void run() {
                        try {
                            BufferedReader br = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                            PrintWriter pw = new PrintWriter(clientSocket.getOutputStream(), true);
                            pw.print("hotelgw>");
                            pw.flush();
                            String input;
                            while ((input = br.readLine()) != null) {
                                System.out.println("input received: " + input);
                                //PROCESS INPUT
                                if (input.equals("h")) {
                                    pw.println(HELP_MESSAGE);
                                } else if (input.equals("l")) {
                                    int[] guests = hotel.listRooms();
                                    System.out.println(guests[0] + "\t" + guests[1] + "\t" + guests[2]);
                                    pw.println(guests[0] + "\t" + guests[1] + "\t" + guests[2]);
                                } else if (input.equals("g")) {
                                    String[] arr = hotel.listGuests();
                                    System.out.println("g after rmi call");
                                    for (int i = 0; i < arr.length; i++) {
                                        pw.println(arr[i]);
                                    }
                                } else if (input.charAt(0) == 'b') {
                                    //parse the arguments
                                    String[] args = input.split(" ");
                                    if (args.length < 3) { //-b type name   There are 3 arguments to be checked
                                        pw.println(HELP_MESSAGE);
                                    } else {
                                        int type = Integer.valueOf(args[1]);
                                        String name = "";
                                        for (int i = 2; i < args.length; i++) {
                                            name += args[i] + " ";
                                        }
                                        name = name.substring(0, name.length() - 1);
                                        int result = hotel.bookRoom(type, name);
                                        if (result < 0) {
                                            pw.println("NA");
                                        } else {
                                            pw.println("ok");
                                        }
                                    }
                                } else if (input.equals("q")) {
                                    pw.println("$");
                                    clientSocket.close();
                                    return;
                                } else {
                                    pw.println(HELP_MESSAGE);
                                }
                                //WRITE BACK TO CLIENT
                                pw.print("hotelgw>");
                                pw.flush();
                            }
                        } catch (IOException io) {
                            io.printStackTrace();
                        }
                    }
                };
                task.run();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}