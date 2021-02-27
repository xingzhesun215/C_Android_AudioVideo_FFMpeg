package com.sun.eg3;

public class Test {
    int age;

    public Test(){

    }

    public Test(int age) {
        this.age = age;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    @Override
    public String toString() {
        return "Test{" +
                "age=" + age +
                '}';
    }
}
