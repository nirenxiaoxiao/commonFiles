package com.caozeal.myArea;

import java.io.IOException;
import java.util.ArrayList;

import javax.annotation.PostConstruct;
import javax.servlet.http.HttpServletRequest;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;

import com.caozeal.mappers.LogIpMapper;


/**.
 * 我的领域内第一片大陆
 * <p>创建时间：2017-2-23下午6:04:59
 * <p>com.caozeal.myArea
 * @author  caozhiyong
 * @version 1.0
 */
@Controller
@RequestMapping("/index")
public class TheFirstLand {

    @Autowired
    private SqlSessionFactory sqlSessionFactory;
    
    private static Log logger = LogFactory.getLog(TheFirstLand.class);

        /**
         * init
         * 构建SqlSessionFactory实例
         * <p>
         * 创建时间：2017-2-24上午10:40:54
         * @author caozhiyong
         * @throws IOException
         */
        @PostConstruct
        public void init(){
            sqlSessionFactory.getConfiguration().addMapper(LogIpMapper.class);
        }

    //查询用户  
    @RequestMapping("/ip")  
    public String list(HttpServletRequest request,Model model){
        SqlSession session = sqlSessionFactory.openSession();
        try{
            LogIpMapper mapper = session.getMapper(LogIpMapper.class);
            TLogIp ip = (TLogIp)mapper.selectIp("2400");
            ArrayList<String> cBhList = (ArrayList<String>)session.<String> selectList("com.caozeal.Mapper.selectIp2", 2400);
            TLogIp ip0 = (TLogIp)session.selectOne("com.caozeal.Mapper.selectIp0", 2400);
            TLogIp ip1 = (TLogIp)session.selectOne("com.caozeal.Mapper.selectIp1", 2400);
            LayyIpLog layyIpLog = (LayyIpLog)session.selectOne("com.caozeal.Mapper.selectOneLayyIpLog");
            model.addAttribute("ip", ip.getCIp());  
            model.addAttribute("ip", ip1.getCIp());  
            return "ip";
        }catch(Exception e){
            logger.error(e.getMessage(), e);
            return "";
        }finally{
            session.close();
        }
    }
 }
