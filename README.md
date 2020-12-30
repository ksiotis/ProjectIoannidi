PART 2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        Ανάπτυξη Λογισμικού για Πληροφοριακά Συστήματα 2020-2021 (2o μέρος)

Μέλη Ομάδας:
    Νάνας Ιωάννης 		201700102
    Σιώτης Κωνσταντίνος 201700140

Δομές Δεδομένων:
    Matrix:
        Ένας απλός δισδιάστατος πίνακας από floats, που κρατάει το μέγεθός του.

    Tf-Idf:
        Η ιδέα είναι η εξής. Αξιοποίηση των hashtable ώστε να μην κρατάμε πολύ πληροφορία, αλλά και για γρήγορη προσπέλαση δεδομένων.
        Υπαρχει η class Index που στην ουσία αρχειοθετεί IndexObject στοιχεια.
            Ένα IndexObject, αναπαριστά μία λέξη, την idf τιμη της, το Nt (οπως αναφέρεται στην εργασία) , αλλά και την θέση της λέξης στον tf-idf vector. (aka dimension)
            Οπότε, όταν θέλουμε να κατασκευάσουμε ενα διάνυσμα απο ένα json/id, για καθε λέξη του, ανατρέχουμε το Index.
        Δόμηση των json:
            Όπως στο index, έτσι και για τα jsons αξιοποιούνται hashtables. Ένα json_index class "κρατάει" το id του και ενα hashtable με όλα τα json_indexObject classes που του ανοίκουν.
                json_indexObject κρατάει μια λεξη, ποσες φορές έχει εμφανιστεί στο json αυτο, και την TF τιμή του.
        Για προσπέλαση στα json_index classes αξιοποιείται η hashtable καθώς μπορεί γρήγορα να βρει αυτο που ψάχνει με αναζήτηση στο id.
    

    Logistic Regression:
        Η logistic regression περιέχει 2 πίνακες: τα βάρη της κάθε διάστασης στο tfidf και ένα bias. 
        Χρησιμοποιούνται για την epoch που τα ανανεώνει με βάση τα αποτελέσματα της predict ακολουθώντας τους τύπους του μαθήματος.
        Χρησιμοποιείται απλή gradient descent αλλά υπάρχει δυνατότητα και για batch ή stohastic.
        Υπάρχει δυνατότητα εξαγωγής όλων των παραμέτρων σε και φόρτωσης από αρχείο.


Η εξήγηση των Main προυποθέτει την κατανοηση του PART 1 (παρακάτω).
Main:
    Αφού φτιαχτούν τα specs.
    Χωρίζουμε το δωσμένο csv αρχείο σε training, validation και test Sets ανάλογα.
    Δημιουργούμε τις tf-idf βάσεις ανατρέχοντας το training set.
    Δημιουργούμε τους πίνακες των απολύτων διαφορών των διανυσμάτων που ζητούνται για κάθε data set(|id1-id2|).
    Εκπαιδεύουμε την logistic regression.
    Εξάγουμε τα predictions στο out_pred.csv αρχειο.
    Επίσης εξάγουμε το Index για το idf και το trained model στα index.csv και model.txt, ώστε να μπορόυν να αξιοποιηθούν στην predictmain.
    Καθαρίζουμε τις βάσεις.

    Τρόπος Εκτέλεσης:
        $ make
        $ ./master.out Specs_dataset Spec_pair.csv Output_file (buckets_number)


Predict Main:
    Δημιουργούμε τον classifier φορτώνοντάς τα στοιχεία του από αρχείο.
    Δημιουργούμε τις tf-idf βάσεις φορτώνοντάς τες από αρχείο.
    Δημιουργούμε τους πίνακες των απολύτων διαφορών των διανυσμάτων που ζητούνται για κάθε data set(|id1-id2|) από το αρχείο εισόδου.
    Εξάγουμε τα predictions στο αρχείο εξόδου.

    Τρόπος Εκτέλεσης:
        $ make predictmain
        $ ./predictmain.out Jsons_Path Spec_pair.csv_Path index_output_Filename model_ouput_Filename


Software Testing:
    Γενικά:
        Για την υλοποίηση των tests χρησιμοποιείται η βιβλιοθήκη acutest.
        Όλα τα tests βρίσκονται στον φάκελο Unit_tests.

    Τρόπος Εκτέλεσης:
        Makefile:
            Μέσω της makefile δίνονται διάφορες δυνατότητες. Αυτές είναι:
                $ make         "δημιουργεί όλα τα tests"
                $ make name_test        "δημιουργεί το εκτελέσιμο πρόγραμμα του name_test.cpp"
        Για την εκτέλεση, απλά δίνεται το όνομα του εκτελέσιμου.



###################################################################################################################################
###################################################################################################################################
###################################################################################################################################
###################################################################################################################################
###################################################################################################################################

PART 1
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	Ανάπτυξη Λογισμικού για Πληροφοριακά Συστήματα 2020-2021 (1o μέρος)

Μέλη Ομάδας:
    Νάνας Ιωάννης 		201700102
    Σιώτης Κωνσταντίνος 201700140

Δομές Δεδομένων:
    include/
        list.hpp: Στην list.hpp περιέχεται ο αφαιρετικός τύπος μιας λίστας (με list nodes) που μπορούν να αποθηκεύουν δείκτη οποιουδήποτε τύπου με την χρήση Template.
        avl.hpp: Δέντρο τύπου avl με δεδομένα δείκτες σε ό,τι χρειαζόμαστε.
        hashtable.hpp: Hashtable για strings οπού τα δεδομένα αποθηκεύονται σε avl δέντρο για γρήγορη ταχύτητα αναζήτησης.
        spec.hpp: Περιέχει spec και clique όπου spec("id", clique*) και clique(spec list). Αφού τα cliques έχουν δεδομένα σε λίστα, αξιοποιούνται οι συναρτήσεις της list.hpp (όπως merge δυο κόμβων)

    Γενικά:
        Τα specs αποθηκεύονται και ο τρόπος πρόσβασης τους είναι ο εξής: Hashtable (hashtab) -> avl_tree search
        όπως επίσης και αποθηκεύονται σε λίστα (specContainer).
        Οι κλίκες(cliques) αποθηκεύονται επίσης σε λίστα (cliqueContainer).

Main:
    Αφού δεχτεί τα filenames που χρειάζεται και δημιουργήσει το hashtable και λίστες, διαβάζει τους φακέλους με τα .json αρχεία.
    Κατά την ανάγνωση, δημιουργεί το id και δημιουργεί τα ανάλογα specs, cliques τα οποία μάλιστα και προσθέτει στις ανάλογες δομές (όπως αναφέρονται στην ενότητα "Δομές Δεδομένων").
    Έπειτα, με την χρήση της βάσης, γίνεται η ανάγνωση του .csv την οποία συνοδεύει η ενοποίηση των κλικών όπου αυτές απαιτούνται.
    Τέλος (πριν το καθάρισμα της μνήμης), γίνεται η εγγραφή των ζευγαριών specs (τα id δηλαδή αυτών) στο αρχείο της επιλογής του χρήστη.
        

    Τρόπος Εκτέλεσης:
        $ make
        $ ./master.out Specs_dataset Spec_pair.csv Output_file (buckets_number)



Software Testing:
    Γενικά:
        Για την υλοποίηση των tests χρησιμοποιείται η βιβλιοθήκη acutest.
        Όλα τα tests βρίσκονται στον φάκελο Unit_tests.

    Τρόπος Εκτέλεσης:
        Makefile:
            Μέσω της makefile δίνονται διάφορες δυνατότητες. Αυτές είναι:
                $ make         "δημιουργεί όλα τα tests"
                $ make name_test        "δημιουργεί το εκτελέσιμο πρόγραμμα του name_test.cpp"
        Για την εκτέλεση, απλά δίνεται το όνομα του εκτελέσιμου.



json Parser:
    Δομές Δεδομένων:
        Έχουν χρησιμοποιηθεί οι ακόλουθες κλάσεις:
        data που περιέχει το όνομα(key), αποτελεί την κλάση γονέα για τα:
        - property, περιέχει μια τιμή(value) και χρησιμοποιείται για να αποθηκεύσει την πληροφορία του κάθε json ζευγαριού "key":"value"
        - array, περιέχει μια λίστα από properties, χρησιμοποιείται για να αποθηκεύσει την πληροφορία "key" : ["prop1":"value1", "prop2":"value2", ...]
        jsonObject που περιέχει μια λίστα από data, δηλαδή από properties και arrays και χρησιμοποιείται για να αποθηκεύσει κάθε μέλος του json
        jsonParser που χειρίζεται το διάβασμα από το αρχείο και περιέχει flags, enumerations κ.α.

    Διάβασμα αρχείου:
        Γίνεται έντονη χρήση από flags. Επειδή ακριβώς τα αρχεία .json δεν έχουν συγκεκριμένα lines, αξιοποιείται η ανάγνωση χαρακτήρα-χαρακτήρα.
        Μέσω switch και enum, πραγματοποιείται η αναγνώριση των χαρακτήρων και διαμορφώνουν είτε τα keys είτε τα values (ή και τις list από values).

    Παραδοχές: 
        1.Θεωρούμε πως το κάθε json αρχείο δεν περιέχει συντακτικά λάθη.
        2.Θεωρούμε πως δεν υπάρχουν περισσότερα από 1(ένα) objects  πχ.{{}}. 
