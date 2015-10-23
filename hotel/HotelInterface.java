
public interface HotelInterface extends java.rmi.Remote {
    public int[] listRooms() throws java.rmi.RemoteException;
    public int bookRoom(int type, String name) throws java.rmi.RemoteException;
    public String[] listGuests() throws java.rmi.RemoteException;
}
