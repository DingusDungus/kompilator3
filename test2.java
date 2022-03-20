class BigBoyClass {
    public static void main(String[] args) {
        System.out.println(test.Meth(10,true));
    }
}

class test {
    int number;
    boolean cum;
    int[] pepe;
    public int Meth(int a, boolean b) {
        boolean bob;
        bob = b;
        if (bob) {
            a = a + 1;
        } else {
            a = a + 5;
        }
        return a;
    }
}
