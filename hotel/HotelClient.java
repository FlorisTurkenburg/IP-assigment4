
import java.rmi.Naming;
import gnu.getopt.Getopt;

public class HotelClient {
    public static void main(String args[]) {
        try {

            String HELP_MESSAGE = "For help, use -h flag.\nTo list available rooms: <hostname> -l\nTo book a room: <hostname> -b <type> <name>\nTo list guests: <hostname> -g";

            if (args.length < 2) {
                System.out.println(HELP_MESSAGE);
                return;
            }
            HotelInterface hotel = (HotelInterface) Naming.lookup("rmi://"+args[0]+"/HotelService");
            Getopt g = new Getopt("HotelProg", args, "hlb:g");
            //
            int c;
            String arg;
            while ((c = g.getopt()) != -1)
                switch (c) {
                    case 'h':
                        System.out.println(HELP_MESSAGE);
                        break;
                    case 'l':
                        int[] guests = hotel.listRooms();
                        System.out.println(guests[0] + "\t" + guests[1] + "\t" + guests[2]);
                        break;
                    case 'b':
                        //check if this option has 2 arguments
                        int optind = g.getOptind();
                        if (args.length - optind != 1) {
                            System.out.println("Wrong number of parameters");
                            System.exit(1);
                        }
                        int type = Integer.parseInt(args[optind - 1]);
                        String name = args[optind];
                        int result = hotel.bookRoom(type, name);
                        if (result < 0) {
                            System.out.println("NA");
                        } else {
                            System.out.println("Ok");
                        }
                        break;
                    case 'g':
                        String[] arr = hotel.listGuests();
                        for (int i = 0; i < arr.length; i++) {
                            System.out.println(arr[i]);
                        }
                        break;
                    case '?':
                        break; // getopt() already printed an error
                    //
                    default:
                        System.out.print("getopt() returned " + c + "\n");
                }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}