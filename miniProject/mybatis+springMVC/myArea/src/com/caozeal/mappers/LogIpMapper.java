package com.caozeal.mappers;

import org.apache.ibatis.annotations.Select;

import com.caozeal.myArea.TLogIp;


/**.
 * <p>创建时间：2017-2-28下午2:05:55
 * <p>com.caozeal.myArea
 * @author  caozhiyong
 * @version 1.0
 */
public interface LogIpMapper {
    
    @Select("SELECT * FROM T_LOG_IP " +
    		"WHERE C_JBFY_ID = #{cJbfyId} " +
    		"LIMIT 1")
    TLogIp selectIp(String cJbfyId);
    

}
