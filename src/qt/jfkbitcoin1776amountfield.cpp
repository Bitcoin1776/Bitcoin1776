// Copyright (c) 2011-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "jfkbitcoin1776amountfield.h"

#include "guiconstants.h"
#include "jfkbitcoin1776units.h"
#include "qvaluecombobox.h"

#include <QAbstractSpinBox>
#include <QApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLineEdit>

/** QSpinBox that uses fixed-point numbers internally and uses our own
 * formatting/parsing functions.
 */
class AmountSpinBox : public QAbstractSpinBox {
  Q_OBJECT

public:
  explicit AmountSpinBox(QWidget *parent)
      : QAbstractSpinBox(parent), currentUnit(JFKBitcoin1776Units::JFK),
        singleStep(100000) // satoshis
  {
    setAlignment(Qt::AlignRight);

    connect(lineEdit(), SIGNAL(textEdited(QString)), this,
            SIGNAL(valueChanged()));
  }

  QValidator::State validate(QString &text, int &pos) const {
    if (text.isEmpty())
      return QValidator::Intermediate;
    bool valid = false;
    parse(text, &valid);
    /* Make sure we return Intermediate so that fixup() is called on defocus */
    return valid ? QValidator::Intermediate : QValidator::Invalid;
  }

  void fixup(QString &input) const {
    bool valid = false;
    CAmount val = parse(input, &valid);
    if (valid) {
      input = JFKBitcoin1776Units::format(currentUnit, val, false,
                                          JFKBitcoin1776Units::separatorAlways);
      lineEdit()->setText(input);
    }
  }

  CAmount value(bool *valid_out = 0) const { return parse(text(), valid_out); }

  void setValue(const CAmount &value) {
    lineEdit()->setText(JFKBitcoin1776Units::format(
        currentUnit, value, false, JFKBitcoin1776Units::separatorAlways));
    Q_EMIT valueChanged();
  }

  void stepBy(int steps) {
    bool valid = false;
    CAmount val = value(&valid);
    val = val + steps * singleStep;
    val = qMin(qMax(val, CAmount(0)), JFKBitcoin1776Units::maxMoney());
    setValue(val);
  }

  void setDisplayUnit(int unit) {
    bool valid = false;
    CAmount val = value(&valid);

    currentUnit = unit;

    if (valid)
      setValue(val);
    else
      clear();
  }

  void setSingleStep(const CAmount &step) { singleStep = step; }

  QSize minimumSizeHint() const {
    if (cachedMinimumSizeHint.isEmpty()) {
      ensurePolished();

      const QFontMetrics fm(fontMetrics());
      int h = lineEdit()->minimumSizeHint().height();
      int w = fm.width(JFKBitcoin1776Units::format(
          JFKBitcoin1776Units::JFK, JFKBitcoin1776Units::maxMoney(), false,
          JFKBitcoin1776Units::separatorAlways));
      w += 2; // cursor blinking space

      QStyleOptionSpinBox opt;
      initStyleOption(&opt);
      QSize hint(w, h);
      QSize extra(35, 6);
      opt.rect.setSize(hint + extra);
      extra += hint -
               style()
                   ->subControlRect(QStyle::CC_SpinBox, &opt,
                                    QStyle::SC_SpinBoxEditField, this)
                   .size();
      // get closer to final result by repeating the calculation
      opt.rect.setSize(hint + extra);
      extra += hint -
               style()
                   ->subControlRect(QStyle::CC_SpinBox, &opt,
                                    QStyle::SC_SpinBoxEditField, this)
                   .size();
      hint += extra;
      hint.setHeight(h);

      opt.rect = rect();

      cachedMinimumSizeHint =
          style()
              ->sizeFromContents(QStyle::CT_SpinBox, &opt, hint, this)
              .expandedTo(QApplication::globalStrut());
    }
    return cachedMinimumSizeHint;
  }

private:
  int currentUnit;
  CAmount singleStep;
  mutable QSize cachedMinimumSizeHint;

  /**
   * Parse a string into a number of base monetary units and
   * return validity.
   * @note Must return 0 if !valid.
   */
  CAmount parse(const QString &text, bool *valid_out = 0) const {
    CAmount val = 0;
    bool valid = JFKBitcoin1776Units::parse(currentUnit, text, &val);
    if (valid) {
      if (val < 0 || val > JFKBitcoin1776Units::maxMoney())
        valid = false;
    }
    if (valid_out)
      *valid_out = valid;
    return valid ? val : 0;
  }

protected:
  bool event(QEvent *event) {
    if (event->type() == QEvent::KeyPress ||
        event->type() == QEvent::KeyRelease) {
      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
      if (keyEvent->key() == Qt::Key_Comma) {
        // Translate a comma into a period
        QKeyEvent periodKeyEvent(event->type(), Qt::Key_Period,
                                 keyEvent->modifiers(), ".",
                                 keyEvent->isAutoRepeat(), keyEvent->count());
        return QAbstractSpinBox::event(&periodKeyEvent);
      }
    }
    return QAbstractSpinBox::event(event);
  }

  StepEnabled stepEnabled() const {
    if (isReadOnly()) // Disable steps when AmountSpinBox is read-only
      return StepNone;
    if (text().isEmpty()) // Allow step-up with empty field
      return StepUpEnabled;

    StepEnabled rv = 0;
    bool valid = false;
    CAmount val = value(&valid);
    if (valid) {
      if (val > 0)
        rv |= StepDownEnabled;
      if (val < JFKBitcoin1776Units::maxMoney())
        rv |= StepUpEnabled;
    }
    return rv;
  }

Q_SIGNALS:
  void valueChanged();
};

#include "jfkbitcoin1776amountfield.moc"

JFKBitcoin1776AmountField::JFKBitcoin1776AmountField(QWidget *parent)
    : QWidget(parent), amount(0) {
  amount = new AmountSpinBox(this);
  amount->setLocale(QLocale::c());
  amount->installEventFilter(this);
  amount->setMaximumWidth(170);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->addWidget(amount);
  unit = new QValueComboBox(this);
  unit->setModel(new JFKBitcoin1776Units(this));
  layout->addWidget(unit);
  layout->addStretch(1);
  layout->setContentsMargins(0, 0, 0, 0);

  setLayout(layout);

  setFocusPolicy(Qt::TabFocus);
  setFocusProxy(amount);

  // If one if the widgets changes, the combined content changes as well
  connect(amount, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
  connect(unit, SIGNAL(currentIndexChanged(int)), this, SLOT(unitChanged(int)));

  // Set default based on configuration
  unitChanged(unit->currentIndex());
}

void JFKBitcoin1776AmountField::clear() {
  amount->clear();
  unit->setCurrentIndex(0);
}

void JFKBitcoin1776AmountField::setEnabled(bool fEnabled) {
  amount->setEnabled(fEnabled);
  unit->setEnabled(fEnabled);
}

bool JFKBitcoin1776AmountField::validate() {
  bool valid = false;
  value(&valid);
  setValid(valid);
  return valid;
}

void JFKBitcoin1776AmountField::setValid(bool valid) {
  if (valid)
    amount->setStyleSheet("");
  else
    amount->setStyleSheet(STYLE_INVALID);
}

bool JFKBitcoin1776AmountField::eventFilter(QObject *object, QEvent *event) {
  if (event->type() == QEvent::FocusIn) {
    // Clear invalid flag on focus
    setValid(true);
  }
  return QWidget::eventFilter(object, event);
}

QWidget *JFKBitcoin1776AmountField::setupTabChain(QWidget *prev) {
  QWidget::setTabOrder(prev, amount);
  QWidget::setTabOrder(amount, unit);
  return unit;
}

CAmount JFKBitcoin1776AmountField::value(bool *valid_out) const {
  return amount->value(valid_out);
}

void JFKBitcoin1776AmountField::setValue(const CAmount &value) {
  amount->setValue(value);
}

void JFKBitcoin1776AmountField::setReadOnly(bool fReadOnly) {
  amount->setReadOnly(fReadOnly);
}

void JFKBitcoin1776AmountField::unitChanged(int idx) {
  // Use description tooltip for current unit for the combobox
  unit->setToolTip(unit->itemData(idx, Qt::ToolTipRole).toString());

  // Determine new unit ID
  int newUnit = unit->itemData(idx, JFKBitcoin1776Units::UnitRole).toInt();

  amount->setDisplayUnit(newUnit);
}

void JFKBitcoin1776AmountField::setDisplayUnit(int newUnit) {
  unit->setValue(newUnit);
}

void JFKBitcoin1776AmountField::setSingleStep(const CAmount &step) {
  amount->setSingleStep(step);
}
