import sys
import jwt
from datetime import datetime
from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QLabel, QLineEdit, QPushButton, QTextEdit,
    QHBoxLayout, QFileDialog, QDateEdit, QCheckBox, QMessageBox
)
from PyQt5.QtCore import QDate
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend

class JWTGeneratorApp(QWidget):
    def __init__(self):
        super().__init__()
        self.private_key = None
        self.init_ui()

    def init_ui(self):
        self.setWindowTitle("JWT Generator")
        layout = QVBoxLayout()

        # Load private key
        self.load_key_btn = QPushButton("Load Private Key")
        self.load_key_btn.clicked.connect(self.load_private_key)
        layout.addWidget(self.load_key_btn)

        # Load data from file
        self.load_data_btn = QPushButton("Load Data from File")
        self.load_data_btn.clicked.connect(self.load_data_from_file)
        layout.addWidget(self.load_data_btn)

        # Input fields
        self.fields = {}
        labels = [
            "Motherboard Serial", "HDD Serial", "Windows Product ID",
            "BIOS UUID", "TPM Endorsement Key", "CPU ID"
        ]

        for label in labels:
            if label == "TPM Endorsement Key":
                row = QHBoxLayout()
                lbl = QLabel(label)
                edit = QLineEdit()
                self.fields[label] = edit

                # Checkbox for "Not Available"
                self.tpm_checkbox = QCheckBox("Not Available")
                self.tpm_checkbox.stateChanged.connect(self.toggle_tpm_field)
                row.addWidget(lbl)
                row.addWidget(edit)
                row.addWidget(self.tpm_checkbox)
                layout.addLayout(row)
            else:
                row = QHBoxLayout()
                lbl = QLabel(label)
                edit = QLineEdit()
                self.fields[label] = edit
                row.addWidget(lbl)
                row.addWidget(edit)
                layout.addLayout(row)

        # Date picker for expiration
        self.exp_label = QLabel("Expiration Date")
        self.exp_picker = QDateEdit()
        self.exp_picker.setCalendarPopup(True)
        self.exp_picker.setDate(QDate.currentDate())
        layout.addWidget(self.exp_label)
        layout.addWidget(self.exp_picker)

        # Generate button
        self.generate_btn = QPushButton("Generate JWT")
        self.generate_btn.clicked.connect(self.generate_jwt)
        layout.addWidget(self.generate_btn)

        # Output text area
        self.output = QTextEdit()
        self.output.setReadOnly(True)
        layout.addWidget(self.output)

        self.setLayout(layout)

    def load_private_key(self):
        # Open file dialog to select PEM file
        pem_file, _ = QFileDialog.getOpenFileName(self, "Select Private Key File", "", "PEM Files (*.pem);;All Files (*)")
        if pem_file:
            try:
                with open(pem_file, "rb") as f:
                    self.private_key = serialization.load_pem_private_key(
                        f.read(),
                        password=None,
                        backend=default_backend()
                    )
                # Show MessageBox for successful key load
                QMessageBox.information(self, "Success", f"Private key loaded successfully from: {pem_file}")
            except Exception as e:
                QMessageBox.critical(self, "Error", f"Error loading private key: {e}")

    def toggle_tpm_field(self):
        if self.tpm_checkbox.isChecked():
            self.fields["TPM Endorsement Key"].setText("Not Available")
            self.fields["TPM Endorsement Key"].setDisabled(True)
        else:
            self.fields["TPM Endorsement Key"].clear()
            self.fields["TPM Endorsement Key"].setDisabled(False)

    def load_data_from_file(self):
        try:
            with open("output.txt", "r") as file:
                data = file.readlines()
            # Parse and populate fields
            for line in data:
                line = line.strip().replace(" ", "")  # Remove all spaces
                if line.startswith("Motherboard:"):
                    self.fields["Motherboard Serial"].setText(line.split(":")[1])
                elif line.startswith("HDD:"):
                    self.fields["HDD Serial"].setText(line.split(":")[1])
                elif line.startswith("WinProductId:"):
                    self.fields["Windows Product ID"].setText(line.split(":")[1])
                elif line.startswith("BIOSUUID:"):
                    self.fields["BIOS UUID"].setText(line.split(":")[1])
                elif line.startswith("CPUID:"):
                    self.fields["CPU ID"].setText(line.split(":")[1])
            QMessageBox.information(self, "Success", "Data loaded successfully from output.txt")
        except FileNotFoundError:
            QMessageBox.critical(self, "Error", "File 'output.txt' not found.")
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Error loading data: {e}")

    def generate_jwt(self):
        if not self.private_key:
            QMessageBox.warning(self, "Error", "No private key loaded.")
            return

        try:
            # Collect input data
            payload = {
                "hw_info": {
                    "motherboard_serial": self.fields["Motherboard Serial"].text(),
                    "hdd_serial": self.fields["HDD Serial"].text(),
                    "windows_product_id": self.fields["Windows Product ID"].text(),
                    "bios_uuid": self.fields["BIOS UUID"].text(),
                    "tpm_endorsement_key": self.fields["TPM Endorsement Key"].text(),
                    "cpu_id": self.fields["CPU ID"].text(),
                },
                "exp": int(self.exp_picker.date().startOfDay().toSecsSinceEpoch()),  # Expiration date as timestamp
            }

            # Header
            header = {
                "alg": "RS256",
                "typ": "JWT"
            }

            # Generate JWT
            token = jwt.encode(payload, self.private_key, algorithm="RS256", headers=header)

            # Display the generated JWT
            self.output.setText(token)
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Error generating JWT: {e}")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = JWTGeneratorApp()
    window.show()
    sys.exit(app.exec_())
