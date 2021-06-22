package com.sun.eg1;

public class StuInfo {
    private int stuId;
    private String stuName;
    private int stuAge;
    private String className;

    public StuInfo(int stuId, String stuName, int stuAge, String className) {
        super();
        this.stuId = stuId;
        this.stuName = stuName;
        this.stuAge = stuAge;
        this.className = className;
    }

    @Override
    public String toString() {
        return "StuInfo java{" +
                "stuId=" + stuId +
                ", stuName='" + stuName + '\'' +
                ", stuAge=" + stuAge +
                ", className='" + className + '\'' +
                '}';
    }
}
