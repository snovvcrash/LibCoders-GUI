/**
 * mainwindow.cxx
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

#include <QtWidgets>
#include <QtConcurrentRun>

#include <iostream>
#include <fstream>
#include <cstdint>
#include <utility>
#include <chrono>
#include <linux/limits.h> // PATH_MAX
#include <sys/types.h>    // S_ISREG
#include <sys/stat.h>     // struct stat

#include "mainwindow.hxx"
#include "ui_mainwindow.h"
#include "helpdialog.hxx"
#include "ui_helpdialog.h"
#include "aboutdialog.hxx"
#include "ui_aboutdialog.h"

#include "coders/pcoder.hxx"
#include "coders/bhcoder.hxx"
#include "coders/ahcoder.hxx"
#include "coders/acoder.hxx"

#define ERROR_IS_REGULAR_FILE ( -1)
#define ERROR_FILE_OPEN       ( -2)
#define ERROR_FILE_EXIST      ( -3)

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	connect(ui->browseButton, &QAbstractButton::clicked, this, &MainWindow::browse);
	connect(ui->compressButton, &QAbstractButton::clicked, this, &MainWindow::compress);
	connect(ui->decompressButton, &QAbstractButton::clicked, this, &MainWindow::decompress);
	connect(ui->clearButton, &QAbstractButton::clicked, this, &MainWindow::clear);
	connect(ui->helpButton, &QAbstractButton::clicked, this, &MainWindow::help);
	connect(ui->aboutButton, &QAbstractButton::clicked, this, &MainWindow::about);
	connect(ui->quitButton, &QAbstractButton::clicked, this, &MainWindow::quit);

	connect(ui->inFileNameComboBox, &QComboBox::currentTextChanged, this, &MainWindow::currentInFileNameWatcher);
	connect(&m_compressFutureWatcher, &QFutureWatcher<void>::started, this, &MainWindow::futureWatcherStarted);
	connect(&m_compressFutureWatcher, &QFutureWatcher<void>::finished, this, &MainWindow::futureWatcherFinished);
	connect(&m_compressFutureWatcher, &QFutureWatcher<void>::finished, this, &MainWindow::compressFutureWatcherFinished);
	connect(&m_decompressFutureWatcher, &QFutureWatcher<void>::started, this, &MainWindow::futureWatcherStarted);
	connect(&m_decompressFutureWatcher, &QFutureWatcher<void>::finished, this, &MainWindow::futureWatcherFinished);
	connect(&m_decompressFutureWatcher, &QFutureWatcher<void>::finished, this, &MainWindow::decompressFutureWatcherFinished);

	ui->progressBar->setRange(0, 100);
	ui->progressBar->setValue(100);
	ui->progressBar->setTextVisible(false);
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::currentInFileNameWatcher() {
	QString qStr_inFileName = ui->inFileNameComboBox->currentText();
	QString extension = QFileInfo(qStr_inFileName).fileName().section('.', -1, -1);

	if (extension == "shenn" || extension == "fano" || extension == "huff" ||
		extension == "bhuff" || extension == "ahuff" || extension == "arith") {
		ui->decompressButton->setEnabled(true);
		return;
	}

	ui->decompressButton->setEnabled(false);
}

void MainWindow::futureWatcherStarted() {
	ui->progressBar->setRange(0, 0);
	ui->progressBar->setValue(0);

	ui->browseButton->setEnabled(false);
	ui->compressButton->setEnabled(false);
	ui->decompressButton->setEnabled(false);
	ui->clearButton->setEnabled(false);
	ui->helpButton->setEnabled(false);
	ui->aboutButton->setEnabled(false);
	ui->quitButton->setEnabled(false);
	ui->inFileNameComboBox->setEnabled(false);
	ui->methodComboBox->setEnabled(false);
}

void MainWindow::futureWatcherFinished() {
	ui->progressBar->setRange(0, 100);
	ui->progressBar->setValue(100);

	ui->browseButton->setEnabled(true);
	ui->compressButton->setEnabled(true);
	ui->decompressButton->setEnabled(true);
	ui->clearButton->setEnabled(true);
	ui->helpButton->setEnabled(true);
	ui->aboutButton->setEnabled(true);
	ui->quitButton->setEnabled(true);
	ui->inFileNameComboBox->setEnabled(true);
	ui->methodComboBox->setEnabled(true);
}

void MainWindow::compressFutureWatcherFinished() {
	auto end = std::chrono::steady_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count();

	ui->outputTextEdit->insertPlainText("Done\n");

	QString qStr_inFileName = ui->inFileNameComboBox->currentText();
	QByteArray qB_inFileName = qStr_inFileName.toUtf8();
	char const *inFileName = qB_inFileName.data();
	QString fileName = QFileInfo(qStr_inFileName).fileName();

	QString extension = MainWindow::getCurrentMethod().second;
	QString qStr_outFileName = fileName + extension;
	QByteArray qB_ouFileName = qStr_outFileName.toUtf8();
	char const *outFileName = qB_ouFileName.data();

	std::ifstream inFile;
	if (prepareInputFile(inFileName, inFile))
		return;

	std::ifstream outFile;
	if (prepareInputFile(outFileName, outFile))
		return;

	inFile.seekg(0, std::ios::end);
	uint64_t inSize = inFile.tellg();
	double inSize_Kb = static_cast<double>(inSize) / 1024;

	outFile.seekg(0, std::ios::end);
	uint64_t outSize = outFile.tellg();
	double outSize_Kb = static_cast<double>(outSize) / 1024;

	int ratio = (inSize_Kb - outSize_Kb) / inSize_Kb * 100;

	ui->outputTextEdit->append("Original file:           ");
	ui->outputTextEdit->insertPlainText(qStr_inFileName);
	ui->outputTextEdit->append("Compressed file:   ");
	ui->outputTextEdit->insertPlainText(fileName + extension);
	ui->outputTextEdit->append("STATS");
	ui->outputTextEdit->append("Original file size:           ");
	ui->outputTextEdit->insertPlainText(QString::number(inSize_Kb, 'g', 6));
	ui->outputTextEdit->insertPlainText(" Kbyte");
	ui->outputTextEdit->append("Compressed file size:   ");
	ui->outputTextEdit->insertPlainText(QString::number(outSize_Kb, 'g', 6));
	ui->outputTextEdit->insertPlainText(" Kbyte");
	ui->outputTextEdit->append("Compression ratio:      ");
	ui->outputTextEdit->insertPlainText(QString::number(ratio));
	ui->outputTextEdit->insertPlainText("%");
	ui->outputTextEdit->append("Time taken:                   ");
	ui->outputTextEdit->insertPlainText(QString::number(diff));
	ui->outputTextEdit->insertPlainText(" milliseconds");
	ui->outputTextEdit->append("------------------------------------------------------------");
	ui->outputTextEdit->insertPlainText("--------------------------------------------------------");

	inFile.close();
	outFile.close();
}

void MainWindow::decompressFutureWatcherFinished() {
	auto end = std::chrono::steady_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count();

	ui->outputTextEdit->insertPlainText("Done\n");

	QString qStr_inFileName = ui->inFileNameComboBox->currentText();
	QString fileName = QFileInfo(qStr_inFileName).fileName().section('.', 0, -2);

	QString qStr_outFileName = fileName;
	QByteArray qB_ouFileName = qStr_outFileName.toUtf8();
	char const *outFileName = qB_ouFileName.data();

	std::ifstream outFile;
	if (prepareInputFile(outFileName, outFile))
		return;

	outFile.seekg(0, std::ios::end);
	uint64_t outSize = outFile.tellg();
	double outSize_Kb = static_cast<double>(outSize) / 1024;

	ui->outputTextEdit->append("Original file:               ");
	ui->outputTextEdit->insertPlainText(qStr_inFileName);
	ui->outputTextEdit->append("Decompressed file:   ");
	ui->outputTextEdit->insertPlainText(fileName);
	ui->outputTextEdit->append("STATS");
	ui->outputTextEdit->append("Decompressed file size:   ");
	ui->outputTextEdit->insertPlainText(QString::number(outSize_Kb, 'g', 6));
	ui->outputTextEdit->insertPlainText(" Kbyte");
	ui->outputTextEdit->append("Time taken:                        ");
	ui->outputTextEdit->insertPlainText(QString::number(diff));
	ui->outputTextEdit->insertPlainText(" milliseconds");
	ui->outputTextEdit->append("------------------------------------------------------------");
	ui->outputTextEdit->insertPlainText("--------------------------------------------------------");

	outFile.close();
}

void MainWindow::browse() {
	QString inFileName = QFileDialog::getOpenFileName(
		this,
		tr("Open File"),
		QDir::currentPath(),
		tr("TXT (*.txt);;"
		   "BMP (*.bmp);;"
		   "JPEG (*.jpg);;"
		   "Packed Files (*.shenn *.fano *.huff *.bhuff *.ahuff *.arith);;"
		   "All Files (*.*)")
	);

	if (!inFileName.isEmpty()) {
		if (ui->inFileNameComboBox->findText(inFileName) == -1)
			ui->inFileNameComboBox->addItem(inFileName);
		ui->inFileNameComboBox->setCurrentIndex(ui->inFileNameComboBox->findText(inFileName));
	}
}

void MainWindow::perfomAction(int inv=0) {
	std::pair<int, QString> ret = MainWindow::getCurrentMethod();
	int method = ret.first;
	QString extension = ret.second;

	if (inv) extension = "";

	QString qStr_inFileName = ui->inFileNameComboBox->currentText();
	QByteArray qB_inFileName = qStr_inFileName.toUtf8();
	char const *inFileName = qB_inFileName.data();

	QString fileName = QFileInfo(qStr_inFileName).fileName();
	if (inv)
		fileName = QFileInfo(qStr_inFileName).fileName().section('.', 0, -2);

	QString qStr_outFileName = fileName + extension;
	QByteArray qB_ouFileName = qStr_outFileName.toUtf8();
	char const *outFileName = qB_ouFileName.data();

	std::ifstream inFile;
	if (prepareInputFile(inFileName, inFile))
		return;

	std::ofstream outFile;
	if (prepareOutputFile(outFileName, outFile))
		return;

	switch (method) {
	case 1: {
		if (!inv) {
			staticcodes::pcoder<staticcodes::shennon> shenn(inFile, outFile);
		}
		else
			staticcodes::pdecoder<staticcodes::shennon> shenn(inFile, outFile);
		break;
	}
	case 2: {
		if (!inv)
			staticcodes::pcoder<staticcodes::fano> fano(inFile, outFile);
		else
			staticcodes::pdecoder<staticcodes::fano> fano(inFile, outFile);
		break;
	}
	case 3: {
		if (!inv)
			staticcodes::pcoder<staticcodes::huffman> huff(inFile, outFile);
		else
			staticcodes::pdecoder<staticcodes::huffman> huff(inFile, outFile);
		break;
	}
	case 4: {
		if (!inv)
			contextcodes::bhcoder bhuff(inFile, outFile);
		else
			contextcodes::bhdecoder bhuff(inFile, outFile);
		break;
	}
	case 5: {
		if (!inv)
			adaptivecodes::ahcoder ahuff(inFile, outFile);
		else
			adaptivecodes::ahdecoder ahuff(inFile, outFile);
		break;
	}
	case 6: {
		if (!inv)
			staticcodes::acoder arith(inFile, outFile);
		else
			staticcodes::adecoder arith(inFile, outFile);
		break;
	}
	default:
		break;
	}

	inFile.close();
	outFile.close();
}

void MainWindow::compress() {
	ui->outputTextEdit->append("Compressing, please wait... ");

	m_start = std::chrono::steady_clock::now();
	QFuture<void> future = QtConcurrent::run(this, &MainWindow::perfomAction, 0);
	m_compressFutureWatcher.setFuture(future);
}

void MainWindow::decompress() {
	ui->outputTextEdit->append("Decompressing, please wait... ");

	m_start = std::chrono::steady_clock::now();
	QFuture<void> future = QtConcurrent::run(this, &MainWindow::perfomAction, 1);
	m_decompressFutureWatcher.setFuture(future);
}

void MainWindow::clear() {
	ui->outputTextEdit->clear();
}

void MainWindow::help() {
	HelpDialog help;
	help.exec();
}

void MainWindow::about() {
	AboutDialog about;
	about.exec();
}

void MainWindow::quit() {
	QCoreApplication::quit();
}

std::pair<int, QString> MainWindow::getCurrentMethod() {
	if (ui->methodComboBox->currentText() == "Shennon")
		return std::make_pair(1, ".shenn");
	else if (ui->methodComboBox->currentText() == "Fano")
		return std::make_pair(2, ".fano");
	else if (ui->methodComboBox->currentText() == "Huffman")
		return std::make_pair(3, ".huff");
	else if (ui->methodComboBox->currentText() == "Bigram Huffman")
		return std::make_pair(4, ".bhuff");
	else if (ui->methodComboBox->currentText() == "Adaptive Huffman")
		return std::make_pair(5, ".ahuff");
	else if (ui->methodComboBox->currentText() == "Arithmetic")
		return std::make_pair(6, ".arith");

	return std::make_pair(0, "");
}

int MainWindow::isRegularFile(char const *path) {
	struct stat s;
	stat(path, &s);
	return S_ISREG(s.st_mode);
}

int MainWindow::prepareInputFile(char const *inFileName, std::ifstream& inFile) {
	if (!isRegularFile(inFileName)) {
		std::cerr << "main: No such input file or input file is not a regular file, rerun with -h for help" << std::endl;
		return ERROR_IS_REGULAR_FILE;
	}

	inFile.open(inFileName, std::ios::binary);

	if (!inFile.is_open()) {
		std::cerr << "main: " << std::strerror(errno) << std::endl;
		return ERROR_FILE_OPEN;
	}

	return 0;
}

int MainWindow::prepareOutputFile(char const *outFileName, std::ofstream& outFile) {
	/* if (std::ifstream(outFileName)) {
		std::cout << "main: File already exists" << endl;
		return ERROR_FILE_EXIST;
	} */

	outFile.open(outFileName, std::ios::binary);

	if (!outFile.is_open()) {
		std::cerr << "main: " << std::strerror(errno) << std::endl;
		return ERROR_FILE_OPEN;
	}

	return 0;
}
