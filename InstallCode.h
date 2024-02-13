#pragma once

class Install {
public:
	static void InstallerThread();
	static int ApplyImage();
	static void InstallMain();
	static void RestartSoon();

	class Progress {
	public:
		static void CopyingFiles();
		static void ExpandingFiles();
		static void InstallingFeatures();
		static void InstallingUpdates();
	};
};