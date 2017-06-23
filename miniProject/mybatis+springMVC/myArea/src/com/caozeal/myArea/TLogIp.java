package com.caozeal.myArea;

import java.sql.Timestamp;

/**.
 * <p>创建时间：2017-2-27下午2:13:38
 * <p>com.caozeal.myArea
 * @author  caozhiyong
 * @version 1.0
 */
//@Alias("TLogIp")
public class TLogIp {

    // Fields

    private String CBh;
    private String CIp;
    private String CAddr;
    private Integer NYwlx;
    private String CYwlx;
    private String CYwid;
    private String CYsfyId;
    private String CYsfyName;
    private String CJbfyId;
    private String CJbfyName;
    private Timestamp DJlsj;

    // Constructors

    /** default constructor */
    public TLogIp() {
    }

    public TLogIp(String CBh,Integer NYwlx){
        this.CBh = CBh ;
        this.NYwlx = NYwlx ;
        this.CYwlx = "随意的构造";
    }
    /** minimal constructor */
    public TLogIp(String CBh, String CIp, Integer NYwlx, String CYwlx, String CYwid) {
        this.CBh = CBh;
        this.CIp = CIp;
        this.NYwlx = NYwlx;
        this.CYwlx = CYwlx;
        this.CYwid = CYwid;
    }

    /** full constructor */
    public TLogIp(String CBh, String CIp, String CAddr, Integer NYwlx, String CYwlx, String CYwid,
            String CYsfyId, String CYsfyName, String CJbfyId, String CJbfyName) {
        this.CBh = CBh;
        this.CIp = CIp;
        this.CAddr = CAddr;
        this.NYwlx = NYwlx;
        this.CYwlx = CYwlx;
        this.CYwid = CYwid;
        this.CYsfyId = CYsfyId;
        this.CYsfyName = CYsfyName;
        this.CJbfyId = CJbfyId;
        this.CJbfyName = CJbfyName;
    }

    // Property accessors

    public String getCBh() {
        return this.CBh;
    }

    public void setCBh(String CBh) {
        this.CBh = CBh;
    }

    public String getCIp() {
        return this.CIp;
    }

    public void setCIp(String CIp) {
        this.CIp = CIp;
    }

    public String getCAddr() {
        return this.CAddr;
    }

    public void setCAddr(String CAddr) {
        this.CAddr = CAddr;
    }

    public Integer getNYwlx() {
        return this.NYwlx;
    }

    public void setNYwlx(Integer NYwlx) {
        this.NYwlx = NYwlx;
    }

    public String getCYwlx() {
        return this.CYwlx;
    }

    public void setCYwlx(String CYwlx) {
        this.CYwlx = CYwlx;
    }

    public String getCYwid() {
        return this.CYwid;
    }

    public void setCYwid(String CYwid) {
        this.CYwid = CYwid;
    }

    public String getCYsfyId() {
        return this.CYsfyId;
    }

    public void setCYsfyId(String CYsfyId) {
        this.CYsfyId = CYsfyId;
    }

    public String getCYsfyName() {
        return this.CYsfyName;
    }

    public void setCYsfyName(String CYsfyName) {
        this.CYsfyName = CYsfyName;
    }

    public String getCJbfyId() {
        return this.CJbfyId;
    }

    public void setCJbfyId(String CJbfyId) {
        this.CJbfyId = CJbfyId;
    }

    public String getCJbfyName() {
        return this.CJbfyName;
    }

    public void setCJbfyName(String CJbfyName) {
        this.CJbfyName = CJbfyName;
    }

    /**
     * @return the dJlsj
     */
    public Timestamp getDJlsj() {
        return DJlsj;
    }

    /**
     * @param dJlsj the dJlsj to set
     */
    public void setDJlsj(Timestamp dJlsj) {
        DJlsj = dJlsj;
    }
    
}
