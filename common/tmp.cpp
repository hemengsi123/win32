
/* µÝ¹é´©¼þÎÄ¼þ¼Ð */
if (::PathFileExists(configPath) == FALSE)
{
	vector<string>    vPaths;
	do {
		vPaths.push_back(configPath);
		::PathRemoveFileSpec(configPath);
	} while (::PathFileExists(configPath) == FALSE);

	for (INT i = vPaths.size()-1; i >= 0; i--)
	{
		_tcscpy(configPath, vPaths[i].c_str());
		::CreateDirectory(configPath, NULL);
	}
	vPaths.clear();
}