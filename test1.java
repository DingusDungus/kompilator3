class BigBoyClass {
    public static void main(String[] args) {
        System.out.println(test.Meth(69,true));
    }
}

class test {
    int number;
    boolean cum;
    int[] pepe;
    public int Meth(int a, boolean b) {
        boolean bob;
        bob = b;
        b = this.SetHas_Left(cum);
        bob = this.SetHas_Left(bob);
        a = a + 1;
        return a;
    }
    public boolean SetHas_Left(boolean val) {
        number = 5;
        if (number > 5) {
            val = false;
        } else {
            val = true;
            number = test.Meth(number, val);
        }
        return val;
    }
}
