
import java.util.ArrayList;
import gnu.getopt.Getopt;

public class HotelImp extends java.rmi.server.UnicastRemoteObject implements HotelInterface {

    private int type1num = 10;
    private int type2num = 20;
    private int type3num = 20;

    private ArrayList<Booking> bookings = new ArrayList<Booking>();

    private String HELPMESSAGE = "";

    public HotelImp() throws java.rmi.RemoteException {
        super();
    }
    public String help() throws java.rmi.RemoteException {
        return this.HELPMESSAGE;
    }

    @Override
    public synchronized int[] listRooms() throws java.rmi.RemoteException {
        return new int[]{type1num, type2num, type3num};
    }

    @Override
    public synchronized int bookRoom(int type, String name) throws java.rmi.RemoteException {
        switch (type){
            case 1:
                if(type1num == 0){
                    return -1;
                }
                type1num--;
                break;
            case 2:
                if(type2num == 0){
                    return -1;
                }
                type2num--;
                break;
            case 3:
                if(type3num == 0){
                    return -1;
                }
                type3num--;
                break;
            default: System.exit(1);
        }
        this.bookings.add(new Booking(type, name));
        return 0;
    }

    @Override
    public synchronized String[] listGuests() throws java.rmi.RemoteException {
        System.out.println("inside listguests");
        ArrayList<String> arr = new ArrayList<String>();
        for (int i=0; i<this.bookings.size(); i++){
            arr.add(this.bookings.get(i).getName());
        }
        return arr.toArray(new String[arr.size()]);
    }
}
