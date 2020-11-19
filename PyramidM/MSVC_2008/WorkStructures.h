

struct wizardSettings
{
	wizardSettings()
	{
		palFullPath = "null";
		routeFullPath = "null";
		routUrl = "";
		palUrl = "";
		naviLat = 0;
		naviLon = 0;
		discret = 30;
		saveIni = false;
	}
	QString palFullPath;
	QString routeFullPath;
	QString routUrl;
	QString palUrl;
	double naviLat;
	double naviLon;
	double discret;
	bool saveIni;
};

