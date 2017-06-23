package com.caozeal.myArea;

import java.sql.Timestamp;

/**.
 * <p>创建时间：2017-3-1下午6:25:20
 * <p>com.caozeal.myArea
 * @author  caozhiyong
 * @version 1.0
 */
public class TLayy {

    /**ID  */
    private String CId;
    /**法院ID  */
    private String CCourtId;
    /**法院名称  */
    private String CCourtName;
    /**专业人员ID  */
    private String CProUserId;
    /**专业人员名称  */
    private String CProUserName;
    /**认证情况 1-未认证，2-实名认证，3-认证律师 */
    private Integer NProUserRzqk;
    /**案件类型  */
    private Integer NAjlx;
    /**案件类型  */
    private String CAjlx;
    /**审判程序 1-民事一审 */
    private Integer NSpcx;
    /**审判程序  */
    private String CSpcx;
    /**申请标的金额  */
    private String CSqbdje;
    /**申请标的物  */
    private String CSqbdw;
    /**申请标的行为  */
    private String CSqbdxw;
    /**执行依据类型  */
    private Integer NZxyjlx;
    /**执行依据类型  */
    private String CZxyjlx;
    /**执行依据文书编号  */
    private String CZxyjwsbh;
    /**证件类型  */
    private Integer NIdcardType;
    /**证件号码  */
    private String CIdcard;
    /**手机号码  */
    private String CSjhm;
    /**诉讼请求  */
    private String CSsqq;
    /**案情说明  */
    private String CAqsm;
    /**当事人  */
    private String CDsr;
    /**状态 1-待审查 2-审查合格 3-审查不合格 */
    private Integer NStatus;

    private Integer NBhyy;

    private String CBhyy;
    /**再次提交 1-可以，2-不可以 */
    private Integer NZctj;
    /**创建时间  */
    private Timestamp DCreate;
    /**更新时间  */
    private Timestamp DUpdate;
    /**提交时间  */
    private Timestamp DTjsj;
    /**是否发送np系统  */
    private Integer NFs;
    /**发送np系统处理结果 1-成功，2-失败 */
    private Integer NFsjg;
    /**发送np系统处理结果名称  */
    private String CFsjg;
    /**业务系统ID  */
    private String CYwxtId;
    /**业务系统记录ID(np网上立案主键)  */
    private String CYwxtJlid;
    /**业务系统案件编号(np案件主键)  */
    private String CYwxtAjbh;
    /**案号  */
    private String CAh;
    /**案件名称  */
    private String CAjmc;
    /**案由代码  */
    private Integer NAy;
    /**案由名称  */
    private String CAy;
    /**应交诉讼费金额  */
    private Double NYjssfje;
    /**应交其他诉讼费  */
    private Double NYjqtssf;
    /**减交金额  */
    private Double NJjje;
    /**退费金额  */
    private Double NTfje;
    /**已交纳金额  */
    private Double NYjnje;
    /**缓交金额  */
    private Double NHjje;
    /**是否免交 1：免交（只针对受理费），2（不免交） */
    private Integer NSfmj;
    /**是否已正式立案 1-是，2-否 */
    private Integer NYla;
    
    //加入新加字段
    private Integer NSqlx;//申请类型
    private String CSqlx;//申请类型
    private Integer NSqrsf;//申请人身份
    private String CSqrsf;//申请人身份
    private Integer NDlrlx; // 代理人类型
    private String CDlrlx; // 代理人类型
    
    //4.0新加
    /**原审法院ID  */
    private String CYsfyId;
    /**原审法院名称  */
    private String CYsfyName;
    /**原审法院ID  */
    private String CJbfyId;
    /**原审法院名称  */
    private String CJbfyName;
    /**原审案件编号  */
    private String CYsajBh;
    /**原审案件案号  */
    private String CYsajAh;
    /**业务系统原审案件编号  */
    private String CYwxtYsajbh;
    
    private Integer NSsxz;//诉讼性质

    private String CStatus;
    //4.2新增  行政非诉执行案件的字段
    /**法律依据  */
    private String CFlyj;
    
    //wsfy2.3新增，用于np判断是否需要实名认证
    /**申请人来源  1、电子法院   2、律协  3、公诉机关 */
    private Integer NSqrly;
    /**立案预约审核状态  1、待实名认证  2、待审核 */
    private Integer NLayySqzt;
    
    //wsfy2.3新增，用于np上诉送达
    /** 申请人ID*/
    private String CSqrId;
    

    /**
     * 返回ID 
     *
     * @return  ID
     */
    public String getCId() {
        return CId;
    }

    /**
     * 设置ID 
     *
     * @param cId ID
     */
    public void setCId(String cId) {
        CId = cId;
    }

    /**
     * 返回法院ID 
     *
     * @return  法院ID
     */
    public String getCCourtId() {
        return CCourtId;
    }

    /**
     * 设置法院ID 
     *
     * @param cCourtId 法院ID
     */
    public void setCCourtId(String cCourtId) {
        CCourtId = cCourtId;
    }

    /**
     * 返回法院名称 
     *
     * @return  法院名称
     */
    public String getCCourtName() {
        return CCourtName;
    }

    /**
     * 设置法院名称 
     *
     * @param cCourtName 法院名称
     */
    public void setCCourtName(String cCourtName) {
        CCourtName = cCourtName;
    }

    /**
     * 返回专业人员ID 
     *
     * @return  专业人员ID
     */
    public String getCProUserId() {
        return CProUserId;
    }

    /**
     * 设置专业人员ID 
     *
     * @param cProUserId 专业人员ID
     */
    public void setCProUserId(String cProUserId) {
        CProUserId = cProUserId;
    }

    /**
     * 返回专业人员名称 
     *
     * @return  专业人员名称
     */
    public String getCProUserName() {
        return CProUserName;
    }

    /**
     * 设置专业人员名称 
     *
     * @param cProUserName 专业人员名称
     */
    public void setCProUserName(String cProUserName) {
        CProUserName = cProUserName;
    }

    /**
     * 返回认证情况 1-未认证，2-实名认证，3-认证律师
     *
     * @return  认证情况
     */
    public Integer getNProUserRzqk() {
        return this.NProUserRzqk;
    }
    
    /**
     * 设置认证情况 1-未认证，2-实名认证，3-认证律师
     *
     * @param NProUserRzqk 认证情况
     */
    public void setNProUserRzqk(Integer NProUserRzqk) {
        this.NProUserRzqk = NProUserRzqk;
    }

    /**
     * 返回案件类型 
     *
     * @return  案件类型
     */
    public Integer getNAjlx() {
        return NAjlx;
    }

    /**
     * 设置案件类型 
     *
     * @param nAjlx 案件类型
     */
    public void setNAjlx(Integer nAjlx) {
        NAjlx = nAjlx;
    }

    /**
     * 返回案件类型 
     *
     * @return  案件类型
     */
    public String getCAjlx() {
        return CAjlx;
    }

    /**
     * 设置案件类型 
     *
     * @param cAjlx 案件类型
     */
    public void setCAjlx(String cAjlx) {
        CAjlx = cAjlx;
    }

    /**
     * 返回审判程序 1-民事一审
     *
     * @return  审判程序
     */
    public Integer getNSpcx() {
        return NSpcx;
    }

    /**
     * 设置审判程序 1-民事一审
     *
     * @param nSpcx 审判程序
     */
    public void setNSpcx(Integer nSpcx) {
        NSpcx = nSpcx;
    }

    /**
     * 返回审判程序 
     *
     * @return  审判程序
     */
    public String getCSpcx() {
        return CSpcx;
    }

    /**
     * 设置审判程序 
     *
     * @param cSpcx 审判程序
     */
    public void setCSpcx(String cSpcx) {
        CSpcx = cSpcx;
    }

    /**
     * 返回申请标的金额 
     *
     * @return  申请标的金额
     */
    public String getCSqbdje() {
        return CSqbdje;
    }

    /**
     * 设置申请标的金额 
     *
     * @param cSqbdje 申请标的金额
     */
    public void setCSqbdje(String cSqbdje) {
        CSqbdje = cSqbdje;
    }

    /**
     * 返回申请标的物 
     *
     * @return  申请标的物
     */
    public String getCSqbdw() {
        return CSqbdw;
    }

    /**
     * 设置申请标的物 
     *
     * @param cSqbdw 申请标的物
     */
    public void setCSqbdw(String cSqbdw) {
        CSqbdw = cSqbdw;
    }

    /**
     * 返回申请标的行为 
     *
     * @return  申请标的行为
     */
    public String getCSqbdxw() {
        return CSqbdxw;
    }

    /**
     * 设置申请标的行为 
     *
     * @param cSqbdxw 申请标的行为
     */
    public void setCSqbdxw(String cSqbdxw) {
        CSqbdxw = cSqbdxw;
    }

    /**
     * 返回执行依据类型 
     *
     * @return  执行依据类型
     */
    public Integer getNZxyjlx() {
        return NZxyjlx;
    }

    /**
     * 设置执行依据类型 
     *
     * @param nZxyjlx 执行依据类型
     */
    public void setNZxyjlx(Integer nZxyjlx) {
        NZxyjlx = nZxyjlx;
    }

    /**
     * 返回执行依据类型 
     *
     * @return  执行依据类型
     */
    public String getCZxyjlx() {
        return CZxyjlx;
    }

    /**
     * 设置执行依据类型 
     *
     * @param cZxyjlx 执行依据类型
     */
    public void setCZxyjlx(String cZxyjlx) {
        CZxyjlx = cZxyjlx;
    }

    /**
     * 返回执行依据文书编号 
     *
     * @return  执行依据文书编号
     */
    public String getCZxyjwsbh() {
        return CZxyjwsbh;
    }

    /**
     * 设置执行依据文书编号 
     *
     * @param cZxyjwsbh 执行依据文书编号
     */
    public void setCZxyjwsbh(String cZxyjwsbh) {
        CZxyjwsbh = cZxyjwsbh;
    }

    /**
     * 返回证件类型 
     *
     * @return  证件类型
     */
    public Integer getNIdcardType() {
        return NIdcardType;
    }

    /**
     * 设置证件类型 
     *
     * @param nIdcardType 证件类型
     */
    public void setNIdcardType(Integer nIdcardType) {
        NIdcardType = nIdcardType;
    }

    /**
     * 返回证件号码 
     *
     * @return  证件号码
     */
    public String getCIdcard() {
        return CIdcard;
    }

    /**
     * 设置证件号码 
     *
     * @param cIdcard 证件号码
     */
    public void setCIdcard(String cIdcard) {
        CIdcard = cIdcard;
    }

    /**
     * 返回手机号码 
     *
     * @return  手机号码
     */
    public String getCSjhm() {
        return CSjhm;
    }

    /**
     * 设置手机号码 
     *
     * @param cSjhm 手机号码
     */
    public void setCSjhm(String cSjhm) {
        CSjhm = cSjhm;
    }

    /**
     * 返回诉讼请求 
     *
     * @return  诉讼请求
     */
    public String getCSsqq() {
        return CSsqq;
    }

    /**
     * 设置诉讼请求 
     *
     * @param cSsqq 诉讼请求
     */
    public void setCSsqq(String cSsqq) {
        CSsqq = cSsqq;
    }

    /**
     * 返回案情说明 
     *
     * @return  案情说明
     */
    public String getCAqsm() {
        return CAqsm;
    }

    /**
     * 设置案情说明 
     *
     * @param cAqsm 案情说明
     */
    public void setCAqsm(String cAqsm) {
        CAqsm = cAqsm;
    }

    /**
     * 返回当事人 
     *
     * @return  当事人
     */
    public String getCDsr() {
        return CDsr;
    }

    /**
     * 设置当事人 
     *
     * @param cDsr 当事人
     */
    public void setCDsr(String cDsr) {
        CDsr = cDsr;
    }

    /**
     * 返回状态 1-待审查 2-审查合格 3-审查不合格
     *
     * @return  状态
     */
    public Integer getNStatus() {
        return NStatus;
    }

    /**
     * 设置状态 1-待审查 2-审查合格 3-审查不合格
     *
     * @param nStatus 状态
     */
    public void setNStatus(Integer nStatus) {
        NStatus = nStatus;
    }

    public Integer getNBhyy() {
        return this.NBhyy;
    }
    
    public void setNBhyy(Integer nBhyy) {
        NBhyy = nBhyy;
    }
    
    public String getCBhyy() {
        return CBhyy;
    }

    public void setCBhyy(String cBhyy) {
        CBhyy = cBhyy;
    }

    /**
     * 返回再次提交 1-可以，2-不可以
     *
     * @return  再次提交
     */
    public Integer getNZctj() {
        return NZctj;
    }

    /**
     * 设置再次提交 1-可以，2-不可以
     *
     * @param nZctj 再次提交
     */
    public void setNZctj(Integer nZctj) {
        NZctj = nZctj;
    }

    /**
     * 返回创建时间 
     *
     * @return  创建时间
     */
    public Timestamp getDCreate() {
        return DCreate;
    }

    /**
     * 设置创建时间 
     *
     * @param dCreate 创建时间
     */
    public void setDCreate(Timestamp dCreate) {
        DCreate = dCreate;
    }

    /**
     * 返回更新时间 
     *
     * @return  更新时间
     */
    public Timestamp getDUpdate() {
        return DUpdate;
    }

    /**
     * 设置更新时间 
     *
     * @param dUpdate 更新时间
     */
    public void setDUpdate(Timestamp dUpdate) {
        DUpdate = dUpdate;
    }

    /**
     * 返回提交时间 
     *
     * @return  提交时间
     */
    public Timestamp getDTjsj() {
        return DTjsj;
    }

    /**
     * 设置提交时间 
     *
     * @param dTjsj 提交时间
     */
    public void setDTjsj(Timestamp dTjsj) {
        DTjsj = dTjsj;
    }

    /**
     * 返回是否发送np系统 
     *
     * @return  是否发送np系统
     */
    public Integer getNFs() {
        return this.NFs;
    }
    
    /**
     * 设置是否发送np系统 
     *
     * @param NFs 是否发送np系统
     */
    public void setNFs(Integer NFs) {
        this.NFs = NFs;
    }

    /**
     * 返回发送np系统处理结果 1-成功，2-失败
     *
     * @return  发送np系统处理结果
     */
    public Integer getNFsjg() {
        return this.NFsjg;
    }
    
    /**
     * 设置发送np系统处理结果 1-成功，2-失败
     *
     * @param NFsjg 发送np系统处理结果
     */
    public void setNFsjg(Integer NFsjg) {
        this.NFsjg = NFsjg;
    }

    /**
     * 返回发送np系统处理结果名称 
     *
     * @return  发送np系统处理结果名称
     */
    public String getCFsjg() {
        return this.CFsjg;
    }
    
    /**
     * 设置发送np系统处理结果名称 
     *
     * @param CFsjg 发送np系统处理结果名称
     */
    public void setCFsjg(String CFsjg) {
        this.CFsjg = CFsjg;
    }

    /**
     * 返回业务系统ID 
     *
     * @return  业务系统ID
     */
    public String getCYwxtId() {
        return this.CYwxtId;
    }
    
    /**
     * 设置业务系统ID 
     *
     * @param CYwxtId 业务系统ID
     */
    public void setCYwxtId(String CYwxtId) {
        this.CYwxtId = CYwxtId;
    }

    /**
     * 返回业务系统记录ID(np网上立案主键) 
     *
     * @return  业务系统记录ID(np网上立案主键)
     */
    public String getCYwxtJlid() {
        return this.CYwxtJlid;
    }
    
    /**
     * 设置业务系统记录ID(np网上立案主键) 
     *
     * @param CYwxtJlid 业务系统记录ID(np网上立案主键)
     */
    public void setCYwxtJlid(String CYwxtJlid) {
        this.CYwxtJlid = CYwxtJlid;
    }

    /**
     * 返回案号 
     *
     * @return  案号
     */
    public String getCAh() {
        return this.CAh;
    }
    
    /**
     * 设置案号 
     *
     * @param CAh 案号
     */
    public void setCAh(String CAh) {
        this.CAh = CAh;
    }

    /**
     * 返回案件名称 
     *
     * @return  案件名称
     */
    public String getCAjmc() {
        return this.CAjmc;
    }
    
    /**
     * 设置案件名称 
     *
     * @param CAjmc 案件名称
     */
    public void setCAjmc(String CAjmc) {
        this.CAjmc = CAjmc;
    }

    /**
     * 返回案由代码 
     *
     * @return  案由代码
     */
    public Integer getNAy() {
        return this.NAy;
    }
    
    /**
     * 设置案由代码 
     *
     * @param NAy 案由代码
     */
    public void setNAy(Integer NAy) {
        this.NAy = NAy;
    }

    /**
     * 返回案由名称 
     *
     * @return  案由名称
     */
    public String getCAy() {
        return this.CAy;
    }
    
    /**
     * 设置案由名称 
     *
     * @param CAy 案由名称
     */
    public void setCAy(String CAy) {
        this.CAy = CAy;
    }

    /**
     * 返回应交诉讼费金额 
     *
     * @return  应交诉讼费金额
     */
    public Double getNYjssfje() {
        return this.NYjssfje;
    }

    /**
     * 设置应交诉讼费金额 
     *
     * @param NYjssfje 应交诉讼费金额
     */
    public void setNYjssfje(Double NYjssfje) {
        this.NYjssfje = NYjssfje;
    }

    /**
     * 返回应交其他诉讼费 
     *
     * @return  应交其他诉讼费
     */
    public Double getNYjqtssf() {
        return this.NYjqtssf;
    }

    /**
     * 设置应交其他诉讼费 
     *
     * @param NYjqtssf 应交其他诉讼费
     */
    public void setNYjqtssf(Double NYjqtssf) {
        this.NYjqtssf = NYjqtssf;
    }

    /**
     * 返回减交金额 
     *
     * @return  减交金额
     */
    public Double getNJjje() {
        return this.NJjje;
    }

    /**
     * 设置减交金额 
     *
     * @param NJjje 减交金额
     */
    public void setNJjje(Double NJjje) {
        this.NJjje = NJjje;
    }

    /**
     * 返回退费金额 
     *
     * @return  退费金额
     */
    public Double getNTfje() {
        return this.NTfje;
    }

    /**
     * 设置退费金额 
     *
     * @param NTfje 退费金额
     */
    public void setNTfje(Double NTfje) {
        this.NTfje = NTfje;
    }

    /**
     * 返回已交纳金额 
     *
     * @return  已交纳金额
     */
    public Double getNYjnje() {
        return this.NYjnje;
    }

    /**
     * 设置已交纳金额 
     *
     * @param NYjnje 已交纳金额
     */
    public void setNYjnje(Double NYjnje) {
        this.NYjnje = NYjnje;
    }

    /**
     * 返回缓交金额 
     *
     * @return  缓交金额
     */
    public Double getNHjje() {
        return this.NHjje;
    }

    /**
     * 设置缓交金额 
     *
     * @param NHjje 缓交金额
     */
    public void setNHjje(Double NHjje) {
        this.NHjje = NHjje;
    }

    /**
     * 返回是否免交 1：免交（只针对受理费），2（不免交）
     *
     * @return  是否免交
     */
    public Integer getNSfmj() {
        return this.NSfmj;
    }

    /**
     * 设置是否免交 1：免交（只针对受理费），2（不免交）
     *
     * @param NSfmj 是否免交
     */
    public void setNSfmj(Integer NSfmj) {
        this.NSfmj = NSfmj;
    }

    /**
     * 返回是否已正式立案 1-是，2-否
     *
     * @return  是否已正式立案
     */
    public Integer getNYla() {
        return NYla;
    }

    /**
     * 设置是否已正式立案 1-是，2-否
     *
     * @param nYla 是否已正式立案
     */
    public void setNYla(Integer nYla) {
        NYla = nYla;
    }

    /**
     * 返回申请类型 
     *
     * @return  申请类型
     */
    public Integer getNSqlx() {
        return NSqlx;
    }

    /**
     * 设置申请类型 
     *
     * @param nSqlx 申请类型
     */
    public void setNSqlx(Integer nSqlx) {
        NSqlx = nSqlx;
    }

    /**
     * 返回申请类型 
     *
     * @return  申请类型
     */
    public String getCSqlx() {
        return CSqlx;
    }

    /**
     * 设置申请类型 
     *
     * @param cSqlx 申请类型
     */
    public void setCSqlx(String cSqlx) {
        CSqlx = cSqlx;
    }

    /**
     * 返回申请人身份 
     *
     * @return  申请人身份
     */
    public Integer getNSqrsf() {
        return NSqrsf;
    }

    /**
     * 设置申请人身份 
     *
     * @param nSqrsf 申请人身份
     */
    public void setNSqrsf(Integer nSqrsf) {
        NSqrsf = nSqrsf;
    }

    /**
     * 返回申请人身份 
     *
     * @return  申请人身份
     */
    public String getCSqrsf() {
        return CSqrsf;
    }

    /**
     * 设置申请人身份 
     *
     * @param cSqrsf 申请人身份
     */
    public void setCSqrsf(String cSqrsf) {
        CSqrsf = cSqrsf;
    }

    /**
     * 返回代理人类型 
     *
     * @return  代理人类型
     */
    public Integer getNDlrlx() {
        return NDlrlx;
    }

    /**
     * 设置代理人类型 
     *
     * @param nDlrlx 代理人类型
     */
    public void setNDlrlx(Integer nDlrlx) {
        NDlrlx = nDlrlx;
    }

    /**
     * 返回代理人类型 
     *
     * @return  代理人类型
     */
    public String getCDlrlx() {
        return CDlrlx;
    }

    /**
     * 设置代理人类型 
     *
     * @param cDlrlx 代理人类型
     */
    public void setCDlrlx(String cDlrlx) {
        CDlrlx = cDlrlx;
    }
    
    /**
     * 返回业务系统案件编号(np案件主键) 
     *
     * @return  业务系统案件编号(np案件主键)
     */
    public String getCYwxtAjbh() {
        return CYwxtAjbh;
    }

    /**
     * 设置业务系统案件编号(np案件主键) 
     *
     * @param cYwxtAjbh 业务系统案件编号(np案件主键)
     */
    public void setCYwxtAjbh(String cYwxtAjbh) {
        CYwxtAjbh = cYwxtAjbh;
    }

    /**
     * 返回原审法院ID 
     *
     * @return  原审法院ID
     */
    public String getCYsfyId() {
        return CYsfyId;
    }

    /**
     * 设置原审法院ID 
     *
     * @param cYsfyId 原审法院ID
     */
    public void setCYsfyId(String cYsfyId) {
        CYsfyId = cYsfyId;
    }

    /**
     * 返回原审法院名称 
     *
     * @return  原审法院名称
     */
    public String getCYsfyName() {
        return CYsfyName;
    }

    /**
     * 设置原审法院名称 
     *
     * @param cYsfyName 原审法院名称
     */
    public void setCYsfyName(String cYsfyName) {
        CYsfyName = cYsfyName;
    }

    /**
     * 返回原审案件编号 
     *
     * @return  原审案件编号
     */
    public String getCYsajBh() {
        return CYsajBh;
    }

    /**
     * 设置原审案件编号 
     *
     * @param cYsajBh 原审案件编号
     */
    public void setCYsajBh(String cYsajBh) {
        CYsajBh = cYsajBh;
    }

    /**
     * 返回原审案件案号 
     *
     * @return  原审案件案号
     */
    public String getCYsajAh() {
        return CYsajAh;
    }

    /**
     * 设置原审案件案号 
     *
     * @param cYsajAh 原审案件案号
     */
    public void setCYsajAh(String cYsajAh) {
        CYsajAh = cYsajAh;
    }

    /**
     * 返回业务系统原审案件编号 
     *
     * @return  业务系统原审案件编号
     */
    public String getCYwxtYsajbh() {
        return CYwxtYsajbh;
    }

    /**
     * 设置业务系统原审案件编号 
     *
     * @param cYwxtYsajbh 业务系统原审案件编号
     */
    public void setCYwxtYsajbh(String cYwxtYsajbh) {
        CYwxtYsajbh = cYwxtYsajbh;
    }

    /**
     * 返回原审法院ID 
     *
     * @return  原审法院ID
     */
    public String getCJbfyId() {
        return CJbfyId;
    }

    /**
     * 设置原审法院ID 
     *
     * @param cJbfyId 原审法院ID
     */
    public void setCJbfyId(String cJbfyId) {
        CJbfyId = cJbfyId;
    }

    /**
     * 返回原审法院名称 
     *
     * @return  原审法院名称
     */
    public String getCJbfyName() {
        return CJbfyName;
    }

    /**
     * 设置原审法院名称 
     *
     * @param cJbfyName 原审法院名称
     */
    public void setCJbfyName(String cJbfyName) {
        CJbfyName = cJbfyName;
    }

    public String getCStatus() {
        return CStatus;
    }

    public void setCStatus(String cStatus) {
        CStatus = cStatus;
    }

    /**
     * 返回法律依据 
     *
     * @return  法律依据
     */
    public String getCFlyj() {
        return CFlyj;
    }

    /**
     * 设置法律依据 
     *
     * @param cFlyj 法律依据
     */
    public void setCFlyj(String cFlyj) {
        CFlyj = cFlyj;
    }

    /**
     * 返回申请人来源
     * @return
     */
    public Integer getNSqrly() {
        return NSqrly;
    }

    /**
     * 设置申请人来源
     * @param nSqrly
     */
    public void setNSqrly(Integer nSqrly) {
        NSqrly = nSqrly;
    }

    /**
     * 返回申请状态
     * @return
     */
    public Integer getNLayySqzt() {
        return NLayySqzt;
    }

    /**
     * 设置申请状态
     * @param nLayySqzt
     */
    public void setNLayySqzt(Integer nLayySqzt) {
        NLayySqzt = nLayySqzt;
    }

    /**
     * 返回申请人ID
     * @return
     */
    public String getCSqrId() {
        return CSqrId;
    }

    /**
     * 设置申请人ID
     * @param cSqrId
     */
    public void setCSqrId(String cSqrId) {
        CSqrId = cSqrId;
    }

    public Integer getNSsxz() {
        return NSsxz;
    }

    public void setNSsxz(Integer nSsxz) {
        NSsxz = nSsxz;
    }
}
