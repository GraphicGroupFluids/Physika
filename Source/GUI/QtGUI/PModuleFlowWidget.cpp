#include "PModuleFlowWidget.h"

//QT
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMenuBar>

//Qt Nodes
#include "Nodes/QtFlowView.h"
#include "Nodes/DataModelRegistry.h"

namespace PhysIKA
{
	PModuleFlowWidget::PModuleFlowWidget(QWidget *parent) :
		QWidget(parent)
	{
		auto menuBar = new QMenuBar();
		auto newAction = menuBar->addAction("New..");
		auto saveAction = menuBar->addAction("Save..");
		auto loadAction = menuBar->addAction("Load..");
		auto clearAction = menuBar->addAction("Clear..");

		QVBoxLayout *l = new QVBoxLayout(this);

		l->addWidget(menuBar);
		module_scene = new QtModuleFlowScene(this);
		l->addWidget(new QtNodes::QtFlowView(module_scene));
		l->setContentsMargins(0, 0, 0, 0);
		l->setSpacing(0);

		QObject::connect(saveAction, &QAction::triggered,
			module_scene, &QtModuleFlowScene::save);

		QObject::connect(loadAction, &QAction::triggered,
			module_scene, &QtModuleFlowScene::load);

		QObject::connect(clearAction, &QAction::triggered,
			module_scene, &QtModuleFlowScene::clearScene);
	}

	PModuleFlowWidget::~PModuleFlowWidget()
	{
	}
}