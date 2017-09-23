/**
 * mainwindow.hxx
 *
 * Part of LibCoders-GUI
 * by snovvcrash
 * 07.2017
 */

/*
 * Copyright (C) 2017 snovvcrash
 *
 * This file is part of LibCoders-GUI.
 *
 * LibCoders-GUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LibCoders-GUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LibCoders-GUI.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QDir>

#include <utility>
#include <chrono>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void currentInFileNameWatcher();
	void futureWatcherFinished();
	void futureWatcherStarted();
	void compressFutureWatcherFinished();
	void decompressFutureWatcherFinished();

	void browse();
	void compress();
	void decompress();
	void clear();
	void help();
	void about();
	void quit();

private:
	Ui::MainWindow *ui;
	QFutureWatcher<void> m_compressFutureWatcher;
	QFutureWatcher<void> m_decompressFutureWatcher;
	std::chrono::time_point<std::chrono::steady_clock> m_start;

	void perfomAction(int inv);

	std::pair<int, QString> getCurrentMethod();
	int isRegularFile(char const* path);
	int prepareInputFile(char const *inFileName, std::ifstream& inFile);
	int prepareOutputFile(char const *outFileName, std::ofstream& outFile);
};

#endif // MAINWINDOW_H
