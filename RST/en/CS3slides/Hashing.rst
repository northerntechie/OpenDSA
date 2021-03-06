.. This file is part of the OpenDSA eTextbook project. See
.. http://opendsa.org for more details.
.. Copyright (c) 2012-2020 by the OpenDSA Project Contributors, and
.. distributed under an MIT open source license.

.. avmetadata::
   :author: Cliff Shaffer

.. slideconf::
   :autoslides: False

=======
Hashing
=======

Hashing
-------

.. slide:: Hashing (1)

   Hashing: The process of mapping a key value to a position in a table.

   A hash function maps key values to positions.  It is denoted by :math:`h`.

   A hash table is an array that holds the records.  It is denoted by **HT**.

   **HT** has :math:`M` slots, indexed form 0 to :math:`M-1`.


.. slide:: Hashing (2)

   For any value :math:`K` in the key range and some hash function
   :math:`h`, :math:`h(K) = i`, :math:`0 <= i < M`, such that
   key(HT[i]) :math:`= K`.

   Hashing is appropriate only for sets (no duplicates).

   Good for both in-memory and disk-based applications.

   Answers the question "What record, if any, has key value K?"


.. slide:: Simple Examples

   .. inlineav:: hashIntroCON ss
      :long_name: Hashing Intro Slideshow
      :links: AV/Hashing/hashIntroCON.css
      :scripts: AV/Hashing/hashIntroCON.js
      :output: show

   * More reasonable example:
      * Store about 1000 records with keys in range 0 to 16,383.
      * Impractical to keep a hash table with 16,384 slots.
      * We must devise a hash function to map the key range to a
        smaller table.


.. slide:: Collisions (1)

   * Given: hash function **h** with keys :math:`k_1` and :math:`k_2`.
     :math:`\beta` is a slot in the hash table.

   * If :math:`\mathbf{h}(k_1) = \beta = \mathbf{h}(k_2)`, then
     :math:`k_1` and :math:`k_2` have a collision at :math:`\beta`
     under **h**.

   * Search for the record with key :math:`K`:
      #. Compute the table location :math:`\mathbf{h}(K)`.
      #. Starting with slot :math:`\mathbf{h}(K)`, locate the record
         containing key :math:`K` using (if necessary) a collision
         resolution policy.


.. slide:: Collisions (2)

   * Collisions are inevitable in most applications.
      * Example: Among 23 people, some pair is likely to share a
        birthday.

   .. avembed:: AV/Hashing/Birthday.html pe


.. slide:: Hash Functions (1)

   * A hash function MUST return a value within the hash table range.

   * To be practical, a hash function SHOULD evenly distribute the
     records stored among the hash table slots.

   * Ideally, the hash function should distribute records with equal
     probability to all hash table slots.  In practice, success
     depends on distribution of actual records stored.


.. slide:: Hash Functions (2)

   * If we know nothing about the incoming key distribution, evenly
     distribute the key range over the hash table slots while avoiding
     obvious opportunities for clustering.

   * If we have knowledge of the incoming distribution, use a
     distribution-dependent hash function.


.. slide:: Simple Mod Function

   ::

      int h(int x) {
        return x % 16;
      }

   .. inlineav:: hashFuncExCON1 ss
      :long_name: Hash Function Slideshow 1
      :links: 
      :scripts: AV/Hashing/hashFuncExCON1.js
      :output: show


.. slide:: Binning

   .. inlineav:: hashFuncExCON2 ss
      :long_name: Hash Function Slideshow 2
      :links: 
      :scripts: AV/Hashing/hashFuncExCON2.js
      :output: show


.. slide:: Mod vs. Binning

   .. odsafig:: Images/HashNormal.png
      :width: 750
      :align: center
      :capalign: center
      :figwidth: 90%
      :alt: Binning vs. Mod Function


.. slide:: Mid-Square Method

   .. odsafig:: Images/MidSquare.png
      :width: 100
      :align: center
      :capalign: justify
      :figwidth: 90%
      :alt: Mid-square method example

   .. avembed:: AV/Hashing/MidSquare.html pe


.. slide:: Strings Function: Character Adding

   ::

      int sascii(String x, int M) {
        char ch[];
        ch = x.toCharArray();
        int xlength = x.length();

        int i, sum;
        for (sum=0, i=0; i < x.length(); i++)
          sum += ch[i];
        return sum % M;
      }

   .. avembed:: AV/Hashing/StringSimple.html pe


.. slide:: String Folding

   .. codeinclude:: Hashing/Hash
      :tag: sfold

   .. avembed:: AV/Hashing/StringSfold.html pe


.. slide:: Open Hashing

   .. inlineav:: openhashCON dgm
      :links: AV/Hashing/openhashCON.css
      :scripts: AV/Hashing/openhashCON.js


.. slide:: Bucket Hashing (1)

   .. inlineav:: buckethashCON1 ss
      :long_name: Bucket Hashing Slideshow 1
      :links: AV/Hashing/buckethashCON.css
      :scripts: AV/Hashing/buckethashCON1.js
      :output: show


.. slide:: Bucket Hashing (2)

   .. inlineav:: buckethashCON2 ss
      :long_name: Bucket Hashing Slideshow 2
      :links: AV/Hashing/buckethashCON.css
      :scripts: AV/Hashing/buckethashCON2.js
      :output: show


.. slide:: Closed Hashing

   * Closed hashing stores all records directly in the hash table.

   * Each record :math:`i` has a home position :math:`\mathbf{h}(k_i)`.

   * If another record occupies the home position for :math:`i`, then
     another slot must be found to store :math:`i`.

   * The new slot is found by a collision resolution policy.

   * Search must follow the same policy to find records not in their
     home slots.


.. slide:: Collision Resolution

   * During insertion, the goal of collision resolution is to find a
     free slot in the table.

   * Probe sequence: The series of slots visited during insert/search
     by following a collision resolution policy.

   * Let :math:`\beta_0 = \mathbf{h}(K)`.
     Let :math:`(\beta_0, \beta_1, ...)` be the series of slots making
     up the probe sequence.


.. slide:: Insertion

   ::

      // Insert e into hash table HT
      void hashInsert(const Key& k, const Elem& e) {
        int home;                     // Home position for e
        int pos = home = h(k);        // Init probe sequence
        for (int i=1; EMPTYKEY != (HT[pos]).key(); i++) {
          pos = (home + p(k, i)) % M; // probe
          if (k == HT[pos].key()) {
            println("Duplicates not allowed");
            return;
          }
        }
        HT[pos] = e;
      }


.. slide:: Search

   ::

      // Search for the record with Key K
      bool hashSearch(const Key& K, Elem& e) const {
        int home;              // Home position for K
        int pos = home = h(K); // Initial position is the home slot
        for (int i = 1;
             (K != (HT[pos]).key()) && (EMPTYKEY != (HT[pos]).key());
             i++)
          pos = (home + p(K, i)) % M; // Next on probe sequence
        if (K == (HT[pos]).key()) {   // Found it
          e = HT[pos];
          return true;
        }
        else return false;            // K not in hash table
      }


.. slide:: Probe Function

   * Look carefully at the probe function p()::

       pos = (home + p(k, i)) % M; // probe

   * Each time p() is called, it generates a value to be added to the
     home position to generate the new slot to be examined.

   * :math:`p()` is a function both of the element's key value, and of
     the number of steps taken along the probe sequence.
     Not all probe functions use both parameters.


.. slide:: Linear Probing (1)

   * Use the following probe function::

      p(K, i) = i;

   * Linear probing simply goes to the next slot in the table.
   * Past bottom, wrap around to the top.

   * To avoid infinite loop, one slot in the table must always be empty.


.. slide:: Linear Probing (2)

   .. inlineav:: linProbeCON1 ss
      :long_name: Linear Probing Slideshow 1
      :links: AV/Hashing/linProbeCON.css
      :scripts: AV/Hashing/linProbeCON1.js
      :output: show


.. slide:: Problem with Linear Probing

   .. inlineav:: linProbeCON2 ss
      :long_name: Linear Probing Slideshow 2
      :links: AV/Hashing/linProbeCON.css
      :scripts: AV/Hashing/linProbeCON2.js
      :output: show

   * The primary goal of a collision resolution mechanism:
      * Give each empty slot of the table an equal probability of
        receiving the next record.


.. slide:: Linear Probing by Steps (1)

   * Instead of going to the next slot, skip by some constant c.
      * Warning: Pick M and c carefully.

   .. inlineav:: collisionCON1 ss
      :long_name: Linear Probing By Steps Slideshow 1
      :links: AV/Hashing/collisionCON.css
      :scripts: AV/Hashing/collisionCON1.js
      :output: show

   * This effectively splits the key range, and the hash table, into
     two halves. This leads to reduced performance.


.. slide:: Linear Probing by Steps (2)

   * The probe sequence SHOULD cycle through all slots of the table.
      * Pick :math:`c` to be relatively prime to :math:`M`.

   .. inlineav:: collisionCON2 ss
      :long_name: Linear Probing By Steps Slideshow 2
      :links: AV/Hashing/collisionCON.css
      :scripts: AV/Hashing/collisionCON2.js
      :output: show


.. slide:: Pseudo-Random Probing (1)

   .. inlineav:: collisionCON3 ss
      :long_name: Pseudo-Random Probing Slideshow
      :links: AV/Hashing/collisionCON.css
      :scripts: AV/Hashing/collisionCON3.js
      :output: show


.. slide:: Pseudo-Random Probing (2)

   .. inlineav:: collisionCON4 ss
      :long_name: Avoiding the Train
      :links: AV/Hashing/collisionCON.css
      :scripts: AV/Hashing/collisionCON4.js
      :output: show


.. slide:: Quadratic Probing

   .. inlineav:: collisionCON5 ss
      :long_name: Quadratic Probing Slideshow
      :links: AV/Hashing/collisionCON.css
      :scripts: AV/Hashing/collisionCON5.js
      :output: show

   .. inlineav:: collisionCON6 ss
      :long_name: Quadratic Probing Problem
      :links: AV/Hashing/collisionCON.css
      :scripts: AV/Hashing/collisionCON6.js
      :output: show


.. slide:: Double Hashing (1)

   .. inlineav:: collisionCON7 ss
      :long_name: Double Hashing Slideshow 2
      :links: AV/Hashing/collisionCON.css
      :scripts: AV/Hashing/collisionCON7.js
      :output: show


.. slide:: Double Hashing (2)

   .. inlineav:: collisionCON8 ss
      :long_name: Double Hashing Slideshow 3
      :links: AV/Hashing/collisionCON.css
      :scripts: AV/Hashing/collisionCON8.js
      :output: show


.. slide:: Analysis of Closed Hashing

   The load factor is :math:`\alpha = N/M` where :math:`N` is the
   number of records currently in the table.

   .. odsafig:: Images/hashplot.png
      :width: 600
      :align: center
      :capalign: justify
      :figwidth: 90%
      :alt: Hashing analysis plot


.. slide:: Deletion

   * Deleting a record must not hinder later searches.

   * We do not want to make positions in the hash table unusable because of
     deletion.

   * Both of these problems can be resolved by placing a special mark in
     place of the deleted record, called a tombstone.

   * A tombstone will not stop a search, but that slot can be used for
     future insertions.


.. slide:: Tombstones (1)

   .. inlineav:: hashdelCON ss
      :long_name: Hash Deletion Slideshow
      :links: 
      :scripts: AV/Hashing/hashdelCON.js
      :output: show


.. slide:: Tombstones (2)

   * Unfortunately, tombstones add to the average path length.

   * Solutions:
      #. Local reorganizations to try to shorten the average path length.
      #. Periodically rehash the table (by order of most frequently accessed
         record).
