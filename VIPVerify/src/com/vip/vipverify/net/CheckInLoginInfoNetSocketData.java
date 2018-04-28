package com.vip.vipverify.net;

import org.json.JSONException;
import org.json.JSONObject;

import com.vip.vipverify.client.ClientUserInfo;

public class CheckInLoginInfoNetSocketData extends NetSocketData {
	private ClientUserInfo info = null;

	public CheckInLoginInfoNetSocketData(ClientUserInfo info) {
		super();
		this.info = info;
	}

	@Override
	protected byte[] prepare_data() {
		// TODO Auto-generated method stub
		byte[] ret_byte = null;
		JSONObject json_object = new JSONObject();
		try {
			json_object.put(Jsonkey.string_transitionid_key, "");
			json_object.put(Jsonkey.string_magicid_key, "");
			JSONObject json_content = new JSONObject();
			json_object.put(Jsonkey.string_content_key, json_content);
			json_content.put(Jsonkey.string_ctype_key, "checkin_login_in");
			JSONObject json_cvalue = new JSONObject();
			json_content.put(Jsonkey.string_cvalue_key, json_cvalue);
			if (info != null) {
				json_cvalue.put(Jsonkey.string_user_name_key, info.getUser_name());
				json_cvalue.put(Jsonkey.string_user_password_key, info.getUser_password());

			}

			String json_string = json_object.toString();
			ret_byte = json_string.getBytes();
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return ret_byte;
	}

}
