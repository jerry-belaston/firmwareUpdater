#include "templateStep.hpp"

#include "ui/toolkit/dataDownloader.hpp"

#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>

namespace firmwareUpdater::ui::view::workspace::templateInfo
{

TemplateStep::TemplateStep(core::type::TemplateInfo::StepInfo const& stepInfo, toolkit::DataDownloader const& dataDownloader, QWidget* parent)
	: toolkit::StepWidget{ parent }
{
    // Gird layout custom
    auto* gridLayout = new QGridLayout{ this };
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(5);
    //gridLayout->setAlignment(Qt::AlignCenter);
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setRowStretch(0, 1);
    gridLayout->setRowStretch(1, 1);

    // Image label custom
    // Stylesheet not centralized in main.qss to not override templates.json user value
    _imageLabel.setStyleSheet("border: 1px solid rgba(255, 255, 255, 50); color: rgba(255, 255, 255, 50); font: 12pt");
    _imageLabel.setText("<image>");
    _imageLabel.wordWrap();
    _imageLabel.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    gridLayout->addWidget(&_imageLabel, 0, 0);
    connect(&dataDownloader, &toolkit::DataDownloader::downloaded, this, [this](auto const data)
    {
        _pixmap.loadFromData(data);
        resizeEvent(nullptr);
    });

    // Description label custom
    auto* descriptionLabel = new QLabel{ this };
    // Stylesheet not centralized in main.qss to not override templates.json user value
    descriptionLabel->setStyleSheet(QString("border: none;") + 
        "color:" + QString::fromStdString(stepInfo.descriptionColor));
    descriptionLabel->setWordWrap(true);
    auto font = descriptionLabel->font();
    font.setPointSize(stepInfo.descriptionSize);
    descriptionLabel->setFont(font);
    descriptionLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    descriptionLabel->setText(QString::fromStdString(stepInfo.descriptionText));
    gridLayout->addWidget(descriptionLabel, 0, 1);

    // Stylesheet not centralized in main.qss to not override templates.json user value
    auto const buttonStyleSheet{ R"(
		QPushButton {
			min-height: 50px;
			max-height: 50px;
			font: 13pt bold;
			background-color: rgb(0, 0, 90);
		}
		QPushButton:hover{
			background-color: rgba(0, 136, 217, 255);
		}
    )" };

    // Previous button custom
    auto* previousButton = new QPushButton{ this };
    previousButton->setText(QString::fromStdString(stepInfo.previousButtonText));
    previousButton->setSizePolicy({ QSizePolicy::Minimum, QSizePolicy::Expanding });
    previousButton->setVisible(stepInfo.previousButtonDisplayed);
    previousButton->setStyleSheet(buttonStyleSheet);
    gridLayout->addWidget(previousButton, 1, 0);
    connect(previousButton, &QPushButton::clicked, this, [this]
    {
        _timer.stop();
        emit TemplateStep::previous();
    });

    // Next button custom
    auto* nextButton = new QPushButton{ this };
    nextButton->setText(QString::fromStdString(stepInfo.nextButtonText));
    nextButton->setSizePolicy({ QSizePolicy::Minimum, QSizePolicy::Expanding });
    // Stylesheet not centralized in main.qss to not override templates.json user value
    nextButton->setStyleSheet(buttonStyleSheet);
    gridLayout->addWidget(nextButton, 1, 1);
    connect(nextButton, &QPushButton::clicked, this, [this]
    {
        _timer.stop();
        emit TemplateStep::next();
    });

    // Timer
    _timer.setInterval(stepInfo.duration.count());
    connect(&_timer, &QTimer::timeout, this, [this]
    {
        _timer.stop();
        emit TemplateStep::next();
    });
}

void TemplateStep::startProcessing()
{
    _timer.start();
}

void TemplateStep::stopProcessing()
{
    _timer.stop();
}

void TemplateStep::resizeEvent(QResizeEvent*)
{
    if (!_pixmap.isNull())
    {
        _imageLabel.clear();
        _imageLabel.setPixmap(_pixmap.scaled(_imageLabel.width(), _imageLabel.height()));
    }
}

} // namespace firmwareUpdater::ui::view::workspace::templateInfo
