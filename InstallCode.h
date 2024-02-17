#pragma once

class Install {
public:
	static void InstallMain();
private:
	static void InstallerThread();
	static int ApplyImage();

	class Progress {
	public:
		static void CopyingFiles();
		static void ExpandingFiles();
		static void InstallingFeatures();
		static void InstallingUpdates();
	};
};