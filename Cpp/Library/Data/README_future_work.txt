This branch reworked CutStatus into an ordered map. CutResult is replaced with a map definition within CutStatus. CutStatus is stored as a member object in DataFrame.
Potential improvements:
- Replace the bool values of the CutStatus map with pair of (bool, string) so a description can be added.
- May need to restore CutResult.hh, and move the map definition there. Could be needed in future Cut.hh

Further Cut work to be done:
- CutFilter.hh and ApplyCut.hh should be updated next.
-- See Nymph/Cpp/Library_v1/Data/KTCutFilter.hh
-- and Nymph/Cpp/Library_v1/Data/KTApplyCut.hh
- Cut.hh may also need updating

