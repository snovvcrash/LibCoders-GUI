/**
 * helpdialog.cxx
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

#include "helpdialog.hxx"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) : QDialog(parent), ui(new Ui::HelpDialog) {
	ui->setupUi(this);
	connect(ui->closeButton, &QAbstractButton::clicked, this, &QDialog::close);
}

HelpDialog::~HelpDialog() {
	delete ui;
}
