
/**
 * Class representing booking of a hotel room
 * type: 1, 2, or 3
 * name: name of the reserver
 */

public class Booking {
    private int type;
    private String name;
    public Booking(int type, String name){
        this.type = type;
        this.name = name;
    }

    public int getType() {
        return type;
    }

    public String getName() {
        return name;
    }
}