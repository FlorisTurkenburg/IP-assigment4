
import java.rmi.Naming;

public class HotelServer {

    public HotelServer(){
        try {
            HotelImp hotel = new HotelImp();
            Naming.rebind("rmi://localhost/HotelService", hotel);
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    public static void main(String args[]){
       new HotelServer();
    }
}
