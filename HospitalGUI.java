import java.awt.*;
import java.util.*;
import javax.swing.*;

class Patient {
    int id;
    String name;
    int severity;

    public Patient(int id, String name, int severity) {
        this.id = id;
        this.name = name;
        this.severity = severity;
    }

    public String toString() {
        return "ID: " + id + ", Name: " + name + ", Severity: " + severity;
    }
}

class Doctor {
    int id;
    String name;
    boolean available;

    public Doctor(int id, String name) {
        this.id = id;
        this.name = name;
        this.available = true;
    }

    public String toString() {
        return "ID: " + id + ", Name: " + name + ", Available: " + available;
    }
}

class Appointment {
    Patient patient;
    Doctor doctor;
    String time;

    public Appointment(Patient patient, Doctor doctor, String time) {
        this.patient = patient;
        this.doctor = doctor;
        this.time = time;
    }

    public String toString() {
        return "Patient: " + patient.name + ", Doctor: " + doctor.name + ", Time: " + time;
    }
}

class BSTNode {
    Patient patient;
    BSTNode left, right;

    public BSTNode(Patient patient) {
        this.patient = patient;
    }
}

class HospitalGUI extends JFrame {
    Queue<Patient> queue = new LinkedList<>();
    ArrayList<Doctor> doctors = new ArrayList<>();
    ArrayList<Appointment> appointments = new ArrayList<>();
    BSTNode bstRoot = null;

    JTextArea displayArea = new JTextArea(15, 40);

    public HospitalGUI() {
        setTitle("Hospital Patient Management System");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(600, 500);
        setLayout(new BorderLayout());

        JPanel buttonPanel = new JPanel(new GridLayout(0, 2));
        JButton addDoctorBtn = new JButton("Add Doctor");
        JButton showDoctorsBtn = new JButton("Show Doctors");
        JButton addPatientBtn = new JButton("Add Patient to Queue");
        JButton processPatientBtn = new JButton("Process Patient to BST");
        JButton showPatientsBtn = new JButton("Show Patients (Inorder)");
        JButton bookAppointmentBtn = new JButton("Book Appointment");
        JButton showAppointmentsBtn = new JButton("Show Appointments");
        
        addDoctorBtn.addActionListener(e -> addDoctor());
        showDoctorsBtn.addActionListener(e -> showDoctors());
        addPatientBtn.addActionListener(e -> addPatient());
        processPatientBtn.addActionListener(e -> processPatient());
        showPatientsBtn.addActionListener(e -> {
            displayArea.setText("Patient Records (Severity Order):\n");
            inorderDisplay(bstRoot);
        });
        bookAppointmentBtn.addActionListener(e -> bookAppointment());
        showAppointmentsBtn.addActionListener(e -> showAppointments());

        buttonPanel.add(addDoctorBtn);
        buttonPanel.add(showDoctorsBtn);
        buttonPanel.add(addPatientBtn);
        buttonPanel.add(processPatientBtn);
        buttonPanel.add(showPatientsBtn);
        buttonPanel.add(bookAppointmentBtn);
        buttonPanel.add(showAppointmentsBtn);

        add(buttonPanel, BorderLayout.NORTH);
        add(new JScrollPane(displayArea), BorderLayout.CENTER);

        setVisible(true);
    }

    void addDoctor() {
        String idStr = JOptionPane.showInputDialog("Enter Doctor ID:");
        String name = JOptionPane.showInputDialog("Enter Doctor Name:");
        int id = Integer.parseInt(idStr);
        doctors.add(new Doctor(id, name));
        displayArea.setText("Doctor added successfully.");
    }

    void showDoctors() {
        StringBuilder sb = new StringBuilder("Available Doctors:\n");
        for (Doctor d : doctors) {
            if (d.available)
                sb.append(d).append("\n");
        }
        displayArea.setText(sb.toString());
    }

    void addPatient() {
        int id = Integer.parseInt(JOptionPane.showInputDialog("Enter Patient ID:"));
        String name = JOptionPane.showInputDialog("Enter Patient Name:");
        int severity = Integer.parseInt(JOptionPane.showInputDialog("Enter Severity (1-10):"));
        queue.add(new Patient(id, name, severity));
        displayArea.setText("Patient added to queue.");
    }

    void processPatient() {
        if (queue.isEmpty()) {
            displayArea.setText("No patients in queue.");
            return;
        }
        Patient p = queue.poll();
        displayArea.setText("Processing patient: " + p);
        bstRoot = insertBST(bstRoot, p);
    }

    void bookAppointment() {
        if (queue.isEmpty()) {
            displayArea.setText("No patients in queue.");
            return;
        }

        Patient p = queue.poll();

        StringBuilder sb = new StringBuilder("Available Doctors:\n");
        for (Doctor d : doctors) {
            if (d.available) sb.append(d.id).append(": ").append(d.name).append("\n");
        }

        String input = JOptionPane.showInputDialog(sb.toString() + "\nEnter Doctor ID:");
        int docId = Integer.parseInt(input);
        Doctor selected = null;
        for (Doctor d : doctors) {
            if (d.id == docId && d.available) {
                selected = d;
                break;
            }
        }

        if (selected == null) {
            displayArea.setText("Invalid or unavailable doctor.");
            return;
        }

        String time = JOptionPane.showInputDialog("Enter Appointment Time:");
        appointments.add(new Appointment(p, selected, time));
        selected.available = false;
        displayArea.setText("Appointment booked successfully.");
    }

    void showAppointments() {
        StringBuilder sb = new StringBuilder("Appointments List:\n");
        for (Appointment a : appointments) {
            sb.append(a).append("\n");
        }
        displayArea.setText(sb.toString());
    }

    BSTNode insertBST(BSTNode root, Patient p) {
        if (root == null) return new BSTNode(p);
        if (p.severity < root.patient.severity)
            root.left = insertBST(root.left, p);
        else
            root.right = insertBST(root.right, p);
        return root;
    }

    void inorderDisplay(BSTNode root) {
        if (root != null) {
            inorderDisplay(root.left);
            displayArea.append(root.patient + "\n");
            inorderDisplay(root.right);
        }
    }

    public static void main(String[] args) {
        new HospitalGUI();
    }
}
