package com.unity3d.ads.android.campaign;

import java.io.File;

import org.json.JSONObject;

import com.unity3d.ads.android.UnityAdsProperties;

import android.util.Log;

public class UnityAdsCampaign {
	
	public enum UnityAdsCampaignStatus { READY, VIEWED, PANIC;
		@Override
		public String toString () {
			String output = name().toString().toLowerCase();
			return output;
		}
		
		public static UnityAdsCampaignStatus getValueOf (String status) {
			if (UnityAdsCampaignStatus.READY.toString().equals(status.toLowerCase()))
				return UnityAdsCampaignStatus.READY;
			else if (UnityAdsCampaignStatus.VIEWED.toString().equals(status.toLowerCase()))
				return UnityAdsCampaignStatus.VIEWED;
			else
				return UnityAdsCampaignStatus.PANIC;
		}
	};
	
	private JSONObject _campaignJson = null;
	private String[] _requiredKeys = new String[]{"v", "s", "id"};
	
	public UnityAdsCampaign () {		
	}
	
	public UnityAdsCampaign (JSONObject fromJSON) {
		_campaignJson = fromJSON;
	}
	
	@Override
	public String toString () {
		return "(ID: " + getCampaignId() + ", STATUS: " + getCampaignStatus().toString() + ", URL: " + getVideoUrl() + ")"; 
	}
	
	public JSONObject toJson () {
		JSONObject retObject = new JSONObject();
		
		try {
			retObject.put("v", getVideoUrl());
			retObject.put("s", getCampaignStatus().toString());
			retObject.put("id", getCampaignId());
		}
		catch (Exception e) {
			Log.d(UnityAdsProperties.LOG_NAME, "Error creating campaign JSON");
			return null;
		}
		
		return retObject;
	}
	
	public String getCampaignId () {
		if (checkDataIntegrity()) {
			try {
				return _campaignJson.getString("id");
			}
			catch (Exception e) {
				Log.d(UnityAdsProperties.LOG_NAME, "getCampaignId: This should not happen!");
			}
		}
		
		return null;
	}
	
	public UnityAdsCampaignStatus getCampaignStatus () {
		if (checkDataIntegrity()) {
			try {
				return UnityAdsCampaignStatus.getValueOf(_campaignJson.getString("s"));
			}
			catch (Exception e) {
				Log.d(UnityAdsProperties.LOG_NAME, "getCampaignStatus: This should not happen!");
			}
		}
		
		return null;
	}
	
	public void setCampaignStatus (UnityAdsCampaignStatus status) {
		if (checkDataIntegrity()) {
			try {
				_campaignJson.put("s", status.toString());
			}
			catch (Exception e) {
				Log.d(UnityAdsProperties.LOG_NAME, "setCampaignStatus: This should not happen!");
			}
		}
	}
	
	public String getVideoUrl () {
		if (checkDataIntegrity()) {
			try {
				return _campaignJson.getString("v");
			}
			catch (Exception e) {
				Log.d(UnityAdsProperties.LOG_NAME, "getVideoUrl: This should not happen!");
			}
		}
		
		return null;
	}
	
	public String getVideoFilename () {
		if (checkDataIntegrity()) {
			try {
				File videoFile = new File(_campaignJson.getString("v"));
				return videoFile.getName();
			}
			catch (Exception e) {
				Log.d(UnityAdsProperties.LOG_NAME, "getVideoFilename: This should not happen!");
			}
		}
		
		return null;
	}
	
	public boolean hasValidData () {
		return checkDataIntegrity();
	}
	
	/* INTERNAL METHODS */
	
	private boolean checkDataIntegrity () {
		if (_campaignJson != null) {
			for (String key : _requiredKeys) {
				if (!_campaignJson.has(key)) {
					return false;
				}
			}
			
			return true;
		}
		return false;
	}
	
	private UnityAdsCampaignStatus getCampaignStatusFromString (String status) {
		return null;
	}
}